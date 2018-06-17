SHELL := /bin/bash

default: clean compile benchmark
all: default

clean:
	rm -rf ./build
	mkdir -p ./build

compile:
	gcc -o ./build/popcount_skylake.o popcount.c -march=skylake
	gcc -S -o ./build/popcount_skylake.s popcount.c -march=skylake
	clang -o ./build/popcount_clang_skylake.o popcount.c -march=skylake
	clang -S -o ./build/popcount_clang_skylake.s popcount.c -march=skylake

	gcc -o ./build/popcountl_skylake.o popcountl.c -march=skylake
	gcc -S -o ./build/popcountl_skylake.s popcountl.c -march=skylake
	clang -o ./build/popcountl_clang_skylake.o popcountl.c -march=skylake
	clang -S -o ./build/popcountl_clang_skylake.s popcountl.c -march=skylake

	gcc -o ./build/popcount_popcnt.o popcount.c -mpopcnt
	gcc -S -o ./build/popcount_popcnt.s popcount.c -mpopcnt
	clang -o ./build/popcount_clang_popcnt.o popcount.c -mpopcnt
	clang -S -o ./build/popcount_clang_popcnt.s popcount.c -mpopcnt

	gcc -o ./build/popcountl_popcnt.o popcountl.c -mpopcnt
	gcc -S -o ./build/popcountl_popcnt.s popcountl.c -mpopcnt
	clang -o ./build/popcountl_clang_popcnt.o popcountl.c -mpopcnt
	clang -S -o ./build/popcountl_clang_popcnt.s popcountl.c -mpopcnt

	gcc -o ./build/popcount.o popcount.c
	gcc -S -o ./build/popcount.s popcount.c
	clang -o ./build/popcount_clang.o popcount.c
	clang -S -o ./build/popcount_clang.s popcount.c

	gcc -o ./build/popcountl.o popcountl.c
	gcc -S -o ./build/popcountl.s popcountl.c
	clang -o ./build/popcountl_clang.o popcountl.c
	clang -S -o ./build/popcountl_clang.s popcountl.c

benchmark:
	@echo "--- standard ---------------------------"
	time ./build/popcount.o
	time ./build/popcountl.o

	@echo "--- Skylake ----------------------------"
	time ./build/popcount_skylake.o
	time ./build/popcountl_skylake.o

	@echo "--- popcnt -----------------------------"
	time ./build/popcount_popcnt.o
	time ./build/popcountl_popcnt.o
