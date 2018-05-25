
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int count = 0;
    uint32_t number = 0b11101010;

    if (__builtin_cpu_supports ("popcnt")){
        asm("popcnt %1,%0" : "=r"(count) : "rm"(number) : "cc");
        printf("POPCNT supported, count of 0b11101010 is %d.", count);
    } else {
        printf("Not supported.");
    }

    return 0;
}