# PostgreSQL popcount function for data type bit(n).
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
-s | `popcount`, `popcount32`, `popcount64`, `popcountAsm` | 8bit lookup table, 32bit hamming weight, 64bit hamming weight, Assembly `POPCNT` instruction.
-t | *integer* | Number of trials.
-a | *integer* | Bit alignment length.
--units | - | Whether time units should be printed or not.
--color | - | Colorize output.

## results
Coming soon!

## generate lookup table
```sh
gcc -o ./helper/bitcount_generator.o ./helper/bitcount_generator.c
./helper/bitcount_generator.o -n 256
```
## check for POPCNT hardware support
```sh
gcc -o ./helper/popcnt.o ./helper/popcnt.c
./helper/popcnt.o
```
