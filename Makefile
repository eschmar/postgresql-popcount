EXTENSION = popcount
DATA = popcount--1.0.0.sql
REGRESS = popcount_test popcount32_test popcount64_test popcountAsm_test popcount256_test
MODULES = popcount
PG_CPPFLAGS = -mpopcnt

# postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
