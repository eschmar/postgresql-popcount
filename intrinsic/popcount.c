#include <stdio.h>

int main() {
    int i, count = 0;
    int val = 0b011101110111011101110111;

    for (i = 0; i < 2000000000; i++) {
        count = __builtin_popcount(val);
        if (count != 18) {
            printf("ERROR!");
            return 0;
        }
    }

    printf("%d", count);
    return 0;
}