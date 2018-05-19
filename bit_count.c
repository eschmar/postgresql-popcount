#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/varbit.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(bit_count);
PG_FUNCTION_INFO_V1(bit_count_32bit);
PG_FUNCTION_INFO_V1(bit_count_64bit);

static const uint64_t m1  = 0x5555555555555555; // 0b0101...
static const uint64_t m2  = 0x3333333333333333; // 0b00110011...
static const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; // 0b00001111...
static const uint64_t h01 = 0x0101010101010101; // the sum of 256 to the power of 0, 1, 2, 3...

// bit count for every 8bit decimal number
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
static int hamming_weight_32bit(int val) {
    // if (val == 0) return 0;
    val = val - ((val >> 1) & 0x55555555);
    val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
    val = (val + (val >> 4)) & 0x0f0f0f0f;
    val = (val * 0x01010101) >> 24;
    return val;
}

/**
 * Hamming weight algorithm using 12 arithmetic operations.
 * 64bit version of the algorithm.
 **/
static int hamming_weight_64bit(uint64_t val)
{
    // if (val == 0) return 0;
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
        count += bitcount[(int) *pointer];
        pointer++;
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
    int length = VARBITLEN(a);
    unsigned char *byte_pointer = VARBITS(a);
    unsigned int *position = (unsigned int *) byte_pointer;

    while (length >= 32) {
        count += hamming_weight_32bit(*position);
        length -= 32;
        position++;
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-32bit-aligned varbit length
    byte_pointer = (unsigned char *) position;
    unsigned char val;
    int remainder = 0;
    int i;

    for (i = (length / 8); i > 0; i--) {
        val = *byte_pointer;
        remainder += ((int) val) << (i * 8);
        byte_pointer++;
    }

    // last byte may not be aligned
    val = *byte_pointer;
    remainder += (int) (val >> (8 - (length % 8)));

    count += hamming_weight_32bit(remainder);
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
    int length = VARBITLEN(a);
    unsigned char *byte_pointer = VARBITS(a);
    unsigned long *position = (unsigned long *) byte_pointer;

    while (length >= 64) {
        count += hamming_weight_64bit(*position);
        length -= 64;
        position++;
    }

    if (length == 0) PG_RETURN_INT32(count);

    // special case, non-64bit-aligned varbit length
    byte_pointer = (unsigned char *) position;
    unsigned char val;
    long remainder = 0;
    int i;

    for (i = (length / 8); i > 0; i--) {
        val = *byte_pointer;
        remainder += ((long) val) << (i * 8);
        byte_pointer++;
    }

    // last byte may not be aligned
    val = *byte_pointer;
    remainder += (long) (val >> (8 - (length % 8)));

    count += hamming_weight_64bit(remainder);
    PG_RETURN_INT32(count);
}
