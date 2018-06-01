#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int n = 256;

/**
 * Hamming weight algorithm using 12 arithmetic operations.
 * 32bit version of the algorithm.
 **/
static int hamming_weight_32bit(uint32_t val) {
    val -= ((val >> 1) & 0x55555555);
    val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
    val = (val + (val >> 4)) & 0x0f0f0f0f;
    val = (val * 0x01010101) >> 24;
    return val;
}

void init(int argc, char *argv[]) {
    int flag;

    while ((flag = getopt (argc, argv, "n:")) != -1) {
        switch (flag) {
            case 'n':
                n = atoi(optarg);
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    init(argc, argv);
    uint32_t current = 0x0;

    while (current < n) {
        printf("%d, ", hamming_weight_32bit(current++));
    }

    return 0;
}