#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(bit_count_encode);

Datum
bit_count_encode(PG_FUNCTION_ARGS)
{
    int32 val = PG_GETARG_INT32(0);

    val = val - ((val >> 1) & 0x55555555);
    val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
    val = (val + (val >> 4)) & 0x0f0f0f0f;
    val = (val * 0x01010101) >> 24;

    PG_RETURN_INT32(val);
}
