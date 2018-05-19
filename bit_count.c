#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/varbit.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(bit_count);

static int hamming_weight_32bit(int val) {
    val = val - ((val >> 1) & 0x55555555);
    val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
    val = (val + (val >> 4)) & 0x0f0f0f0f;
    val = (val * 0x01010101) >> 24;
    return val;
}

// mask = (1 << param) - 1;

Datum
bit_count(PG_FUNCTION_ARGS) {
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
