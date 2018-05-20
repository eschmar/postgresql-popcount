# PostgreSQL bit_count function for data type bit(n).
Requires `pg_config` installed.

```sh
make install
make installcheck
```

## benchmarks
Make sure the extension is installed `CREATE EXTENSION bit_count;`.

```sh
sh helper/experiment.sh [-s bit_count -t 10 -l 512 --units --color]
```

option | values | comment
--- | --- | ---
-s | `bit_count`, `bit_count_32bit`, `bit_count_64bit` | 8bit cache lookup, 32bit hamming weight, 64bit hamming weight.
-t | *integer* | Number of trials.
-l | *integer* | Bit alignment length.
--units | - | Whether time units should be printed or not.
--color | - | Colorize output.

## generate lookup table
```sh
cd helper
gcc -o bitcount_generator.o bitcount_generator.c
./bitcount_generator.o -n 256
```

## todo
* [ ] Move hamming weight to lib
* [ ] Avoid redundant code 64bit variant
* [ ] Improve readme
* [ ] Bump v1
