**Note:** PostgreSQL version 14 introduced their own [`bit_count`](https://www.postgresql.org/docs/14/functions-bitstring.html). You may find [their implementation here](https://github.com/postgres/postgres/blob/master/src/port/pg_bitutils.c) (or [here](https://git.postgresql.org/gitweb/?p=postgresql.git;a=blob;f=src/port/pg_bitutils.c;hb=HEAD)) for comparison. It is therefore recommended to only use this repository for postgres versions <14.

# PostgreSQL population count function for data type bit(n).
Provides `popcount`, `popcount32`, `popcount64`, `popcountAsm`, `popcountAsm64` and `popcount256` functions to PostgreSQL. The extension was elected to provide all algorithms to enable a conscious choice on runtime. Statistical benchmark data suggests that `popcountAsm64` should be chosen in most cases.

## usage
The extension is [available on PGXN](https://pgxn.org/dist/popcount/).

```sh
pgxn install popcount
```

If you want to compile it yourself, `pg_config` is required.

```sh
make install
make installcheck
```

## benchmarks
<img src="https://github.com/eschmar/postgresql-popcount/raw/master/img/graph.png" alt="Benchmarks" style="max-width:100%;">

The test bench was running an Intel(R) Xeon(R) Platinum 8168 CPU @ 2.70GHz and can be reproduced the following way. Make sure the extension is installed beforehand `CREATE EXTENSION popcount;`.

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
