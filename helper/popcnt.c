
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int count = 0;

    // int __builtin_popcount (unsigned int)
    // Generates the popcntl machine instruction. 
    unsigned int number = 0b11101010;

    // int __builtin_popcountl (unsigned long)
    // Generates the popcntl or popcntq machine instruction, depending on the size of unsigned long. 
    unsigned long longnumber = 0xFFFFFFFF01;

    // int __builtin_popcountll (unsigned long long)
    // Generates the popcntq machine instruction.
    unsigned long long longlongnumber = 0xFFFFFFFF01;

    if (__builtin_cpu_supports("popcnt")){
        asm("popcnt %1,%0" : "=r"(count) : "rm"(number) : "cc");
        printf("POPCNT supported!\n\n");
        printf("Count of unsigned int 0b11101010 is\n");
        printf(" > Assembly: %d\n", count);
        printf(" > __builtin_popcount: %d\n\n", __builtin_popcount(number));

        printf("Count of unsigned long 0xFFFFFFFF01 is\n");
        printf(" > __builtin_popcountl: %d\n\n", __builtin_popcountll(longnumber));

        printf("Count of unsigned long long 0xFFFFFFFF01 is\n");
        printf(" > __builtin_popcountl: %d\n", __builtin_popcountll(longlongnumber));
    } else {
        printf("Not supported.");
    }

    return 0;
}