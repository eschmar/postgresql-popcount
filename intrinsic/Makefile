SHELL := /bin/bash

default: clean compile benchmark
all: default

clean:
	rm -rf ./build
	mkdir -p ./build

compile:
	gcc -o ./build/popcount_skylake.o popcount.c -march=skylake
	gcc -S -o ./build/popcount_skylake.s popcount.c -march=skylake

	gcc -o ./build/popcountl_skylake.o popcountl.c -march=skylake
	gcc -S -o ./build/popcountl_skylake.s popcountl.c -march=skylake

	gcc -o ./build/popcount_popcnt.o popcount.c -mpopcnt
	gcc -S -o ./build/popcount_popcnt.s popcount.c -mpopcnt

	gcc -o ./build/popcountl_popcnt.o popcountl.c -mpopcnt
	gcc -S -o ./build/popcountl_popcnt.s popcountl.c -mpopcnt

	gcc -o ./build/popcount.o popcount.c
	gcc -S -o ./build/popcount.s popcount.c

	gcc -o ./build/popcountl.o popcountl.c
	gcc -S -o ./build/popcountl.s popcountl.c

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
