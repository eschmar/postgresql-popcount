#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/varbit.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(bit_count);
PG_FUNCTION_INFO_V1(bit_count_32bit);
PG_FUNCTION_INFO_V1(bit_count_32bit_lookup);
PG_FUNCTION_INFO_V1(bit_count_32bit_own);
PG_FUNCTION_INFO_V1(bit_count_64bit);

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
bit_count(PG_FUNCTION_ARGS) {
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
bit_count_32bit(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    uint32_t *position = (uint32_t *) byte_pointer;

    for (; length >= 4; length -= 4) {
        count += hamming_weight_32bit(*position++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-32bit-aligned varbit length
    uint32_t remainder = 0x0;
    byte_pointer = (unsigned char *) position;
    memcpy((void *) &remainder, (void *) position, length);
    count += hamming_weight_32bit(remainder);

    PG_RETURN_INT32(count);
}

/**
 * 32bit Hamming weight / popcount algorithm for counting bits set.
 * Requires additional aligning logic for the last 32bit trunk.
 * Variant not utilizing memcpy.
 **/
Datum
bit_count_32bit_own(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    uint32_t *position = (uint32_t *) byte_pointer;

    for (; length >= 4; length -= 4) {
        count += hamming_weight_32bit(*position++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-32bit-aligned varbit length
    uint32_t remainder = 0x0;
    unsigned char *remainder_offset = (unsigned char *) &remainder;
    byte_pointer = (unsigned char *) position;

    while (length-- > 0) {
        *remainder_offset++ = *byte_pointer++;
    }

    count += hamming_weight_32bit(remainder);
    PG_RETURN_INT32(count);
}

/**
 * 32bit Hamming weight / popcount algorithm for counting bits set.
 * Requires additional aligning logic for the last 32bit trunk.
 * Table lookup variant for the non-aligned remainder.
 **/
Datum
bit_count_32bit_lookup(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    uint32_t *position = (uint32_t *) byte_pointer;

    for (; length >= 4; length -= 4) {
        count += hamming_weight_32bit(*position++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-32bit-aligned varbit length
    byte_pointer = (unsigned char *) position;

    while (length-- > 0) {
        count += bitcount[(int) *byte_pointer++];
    }

    PG_RETURN_INT32(count);
}

/**
 * 64bit Hamming weight / popcount algorithm for counting bits set.
 * Requires additional aligning logic for the last 64bit trunk.
 **/
Datum
bit_count_64bit(PG_FUNCTION_ARGS) {
    VarBit *a = PG_GETARG_VARBIT_P(0);

    int count = 0;
    int length = VARBITBYTES(a);
    unsigned char *byte_pointer = VARBITS(a);
    uint64_t *position = (uint64_t *) byte_pointer;

    for (; length >= 8; length -= 8) {
        count += hamming_weight_64bit(*position++);
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-64bit-aligned varbit length
    uint64_t remainder = 0x0;
    byte_pointer = (unsigned char *) position;
    memcpy((void *) &remainder, (void *) position, length);
    count += hamming_weight_64bit(remainder);

    PG_RETURN_INT32(count);
}
