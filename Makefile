EXTENSION = bit_count
DATA = bit_count--0.0.1.sql
REGRESS = bit_count_test
MODULES = bit_count

# postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)