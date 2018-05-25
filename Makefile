EXTENSION = popcount
DATA = popcount--1.0.0.sql
REGRESS = popcount_test popcount32_test popcount64_test
MODULES = popcount

# postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
