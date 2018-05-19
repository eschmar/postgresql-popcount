# PostgreSQL bit_count function for data type bit(n).
Requires `pg_config` installed.

```sh
make install
make installcheck
```

## generate lookup table
```sh
cd helper
gcc -o bitcount_generator.o bitcount_generator.c
./bitcount_generator.o -n 256
```

## todo
* [ ] Move hamming weight to lib
* [ ] Avoid redundant code 64bit variant
* [ ] Bump v1
