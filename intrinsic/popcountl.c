#include <stdio.h>

int main() {
    int i, count = 0;
    unsigned long long longlongnumber = 0xFFFFFFFF01;

    for (i = 0; i < 2000000000; i++) {
        count = __builtin_popcountl(longlongnumber);
        if (count != 33) {
            printf("ERROR!");
            return 0;
        }
    }

    printf("%d", count);
    return 0;
}