# PostgreSQL popcount function for data type bit(n).
Provides `popcount`, `popcount32`, `popcount64`, `popcountAsm`, `popcountAsm64` and `popcount256` functions to PostgreSQL.

## usage
Requires `pg_config` installed.

```sh
make install
make installcheck
```

## benchmarks
Make sure the extension is installed `CREATE EXTENSION popcount;`.

```sh
./helper/generate_db_postgres.sh -a 512
./benchmark/postgres.sh [-s popcount -t 10 -a 512 --units --color]

./helper/generate_db_mysql.sh -d <database>
./benchmark/mysql.sh [-s popcount -t 10 -d <database>]
```

option | values | comment
--- | --- | ---
-s | `popcount`, `popcount32`, `popcount64`, `popcountAsm`, `popcountAsm64`, `popcount256` | 8bit lookup table, 32bit hamming weight, 64bit hamming weight, 32 bit intrinsic function, 64 bit intrinsic function, unrolled assembly instruction.
-t | *integer* | Number of trials.
-a | *integer* | Bit alignment length.
--units | - | Whether time units should be printed or not.
--color | - | Colorize output.

## results
<img src="https://github.com/eschmar/postgresql-popcount/raw/master/img/graph.png" alt="Benchmarks" style="max-width:100%;">

## generate lookup table
```sh
gcc -o ./helper/lookup_table_generator.o ./helper/lookup_table_generator.c
./helper/lookup_table_generator.o -n 256
```

## check for POPCNT hardware support
```sh
gcc -o ./helper/popcnt_support.o ./helper/popcnt_support.c
./helper/popcnt_support.o
```
