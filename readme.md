# PostgreSQL bit_count function for data type bit(n).
Requires `pg_config` installed.

```sh
make install
make installcheck
```

## benchmarks
Make sure the extension is installed `CREATE EXTENSION bit_count;`.

```sh
./helper/generate_db_postgres.sh -a 512
./benchmark/postgres.sh [-s bit_count -t 10 -a 512 --units --color]

./helper/generate_db_mysql.sh -d <database>
./benchmark/mysql.sh [-s bit_count -t 10 -d <database>]
```

option | values | comment
--- | --- | ---
-s | `bit_count`, `bit_count_32bit`, `bit_count_64bit` | 8bit cache lookup, 32bit hamming weight, 64bit hamming weight.
-t | *integer* | Number of trials.
-a | *integer* | Bit alignment length.
--units | - | Whether time units should be printed or not.
--color | - | Colorize output.

## results
Coming soon!

## generate lookup table
```sh
cd helper
gcc -o bitcount_generator.o bitcount_generator.c
./bitcount_generator.o -n 256
```

## todo
* [ ] Improve readme, add results
* [ ] Bump v1
