#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/varbit.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(popcount);
PG_FUNCTION_INFO_V1(popcount32);
PG_FUNCTION_INFO_V1(popcount64);
PG_FUNCTION_INFO_V1(popcountIntr);
PG_FUNCTION_INFO_V1(popcountIntrL);
PG_FUNCTION_INFO_V1(popcountAsm);
PG_FUNCTION_INFO_V1(popcount256);

static const uint64_t m1  = 0x5555555555555555; // 0b0101...
static const uint64_t m2  = 0x3333333333333333; // 0b00110011...
static const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; // 0b00001111...
static const uint64_t h01 = 0x0101010101010101; // the sum of 256 to the power of 0, 1, 2, 3...

/**
 * Bit count for every 8bit decimal number
 **/
static const int bitcount[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

/**
 * Hamming weight algorithm using 12 arithmetic operations.
 * 32bit version of the algorithm.
 **/
static int hamming_weight_32bit(uint32_t val) {
    val -= ((val >> 1) & 0x55555555);
    val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
    val = (val + (val >> 4)) & 0x0f0f0f0f;
    return (val * 0x01010101) >> 24;
}

/**
 * Hamming weight algorithm using 12 arithmetic operations.
 * 64bit version of the algorithm.
 **/
static int hamming_weight_64bit(uint64_t val)
{
    val -= (val >> 1) & m1;
    val = (val & m2) + ((val >> 2) & m2);
    val = (val + (val >> 4)) & m4;
    return (val * h01) >> 56;
}

/**
 * Cache lookup algorithm for counting bits set.
 **/
Datum
popcount(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);
    unsigned char *pointer = VARBITS(a);
    int length = VARBITBYTES(a);
    int count = 0;

    while (length-- > 0) {
        count += bitcount[(int) *pointer++];
    }

    PG_RETURN_INT32(count);
}

/**
 * 32bit Hamming weight / popcount algorithm for counting bits set.
 * Requires additional aligning logic for the last 32bit trunk.
 **/
Datum
popcount32(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    uint32_t *position = (uint32_t *) byte_pointer;
    uint32_t remainder = 0x0;

    for (; length >= 4; length -= 4) {
        count += hamming_weight_32bit(*position++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-32bit-aligned varbit length
    byte_pointer = (unsigned char *) position;
    memcpy((void *) &remainder, (void *) position, length);
    count += hamming_weight_32bit(remainder);

    PG_RETURN_INT32(count);
}

/**
 * 64bit Hamming weight / popcount algorithm for counting bits set.
 * Requires additional aligning logic for the last 64bit trunk.
 **/
Datum
popcount64(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    uint64_t *position = (uint64_t *) byte_pointer;
    uint64_t remainder = 0x0;

    for (; length >= 8; length -= 8) {
        count += hamming_weight_64bit(*position++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-64bit-aligned varbit length
    byte_pointer = (unsigned char *) position;
    memcpy((void *) &remainder, (void *) position, length);
    count += hamming_weight_64bit(remainder);

    PG_RETURN_INT32(count);
}

/**
 * 32bit Hamming weight / popcount algorithm for counting bits set.
 * Requires additional aligning logic for the last 32bit trunk.
 **/
Datum
popcountIntr(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    unsigned int *position = (unsigned int *) byte_pointer;
    unsigned int remainder = 0x0;

    for (; length >= 4; length -= 4) {
        count += __builtin_popcount(*position++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-32bit-aligned varbit length
    byte_pointer = (unsigned char *) position;
    memcpy((void *) &remainder, (void *) position, length);
    count += __builtin_popcount(remainder);

    PG_RETURN_INT32(count);
}

/**
 * 64bit Hamming weight / popcount algorithm for counting bits set.
 * Requires additional aligning logic for the last 64bit trunk.
 **/
Datum
popcountIntrL(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    unsigned long *position = (unsigned long *) byte_pointer;
    unsigned long remainder = 0x0;

    for (; length >= 8; length -= 8) {
        count += __builtin_popcountl(*position++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-64bit-aligned varbit length
    byte_pointer = (unsigned char *) position;
    memcpy((void *) &remainder, (void *) position, length);
    count += __builtin_popcountl(remainder);

    PG_RETURN_INT32(count);
}

/**
 * POPCNT Assembly instruction.
 * Requires hardware support or will fail.
 **/
Datum
popcountAsm(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0, temp;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    unsigned int *position = (unsigned int *) byte_pointer;
    unsigned int remainder = 0x0;

    if (!__builtin_cpu_supports("popcnt")){
        ereport(ERROR,
            (
                errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                errmsg("POPCNT instruction not supported."),
                errdetail("Support is indicated by CPUID.01H:ECX.POPCNT[Bit 23] flag."),
                errhint("Use popcount[|32|64]().")
            )
        );
    }

    for (; length >= 4; length -= 4) {
        asm("popcnt %1,%0" : "=r"(temp) : "rm"((unsigned int) *position++) : "cc");
        count += temp;
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-32bit-aligned varbit length
    byte_pointer = (unsigned char *) position;
    memcpy((void *) &remainder, (void *) position, length);
    asm("popcnt %1,%0" : "=r"(temp) : "rm"(remainder) : "cc");
    count += temp;

    PG_RETURN_INT32(count);
}

/**
 * ATTENTION - Does not work properly yet!
 * Unrolled POPCNT Assembly instruction for 256bit steps.
 * Requires hardware support or will fail.
 * Falls back to 64 bit Hamming Weight and memcpy for remainder alignment.
 **/
Datum
popcount256(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int i, count = 0;
    uint64_t counts[4];
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    uint64_t *buffer = (uint64_t *) byte_pointer;
    uint64_t remainder = 0x0;

    if (!__builtin_cpu_supports("popcnt")){
        ereport(ERROR,
            (
                errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                errmsg("POPCNT instruction not supported."),
                errdetail("Support is indicated by CPUID.01H:ECX.POPCNT[Bit 23] flag."),
                errhint("Use popcount[|32|64]().")
            )
        );
    }

    for (i = 0; i < 4; i++) counts[i] = 0;

    // Unrolled 64bit POPCNT
    for (; length >= 32; length -= 32) {
        __asm__(
            "popcnt %4, %4 \n\t add %4, %0 \n\t"
            "popcnt %5, %5 \n\t add %5, %1 \n\t"
            "popcnt %6, %6 \n\t add %6, %2 \n\t"
            "popcnt %7, %7 \n\t add %7, %3 \n\t"
            // input/output
            : "+r" (counts[0]), "+r" (counts[1]), "+r" (counts[2]), "+r" (counts[3])
            // inputs
            : "r"  (buffer[0]), "r"  (buffer[1]), "r"  (buffer[2]), "r"  (buffer[3])
            // clobber indicates that the flags register is modified
            : "cc"
        );

        count += counts[0] + counts[1] + counts[2] + counts[3];
        buffer += 4;
    }

    // fall back to 64bit hamming weight
    for (; length >= 8; length -= 8) {
        count += hamming_weight_64bit(*buffer++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-64bit-aligned varbit length
    byte_pointer = (unsigned char *) buffer;
    memcpy((void *) &remainder, (void *) buffer, length);
    count += hamming_weight_64bit(remainder);

    PG_RETURN_INT32(count);
}
