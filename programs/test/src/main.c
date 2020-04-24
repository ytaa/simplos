#include <simplstd.h>
#include <stdio.h>
unsigned int max = 10;
const char *name = "TEST_PROG      ";

void main() {
    printf("%s --------------------\n", name);
    unsigned int i = 0;
    unsigned int j = 1;

    //void (*kernel_fun)(void) = 2147485008;
    //kernel_fun();

    while (1) {
        i++;
        if (i > 3) break;
        j = i * 2;
        printf("%s - i: %u, j: %u, max: %u\n", name, i, j, max);
        sleep(1000);
    }
    printf("------------------------------\n");
}