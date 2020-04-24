#include <simplstd.h>
#include <stdio.h>
unsigned int max = 10;
const char *name = "TEST_PROG2     ";

void main() {
    printf("%s --------------------\n", name);
    unsigned int i = 0;
    unsigned int j = 1;
    while (1) {
        i++;
        j = i * 2;
        if (i % 100000000 == 0)
            printf("%s - i: %u, j: %u, max: %u\n", name, i, j, max);
        //sleep(2000);
    }
    printf("------------------------------\n");
}