#include <simplstd.h>
#include <stdio.h>

const char *name = "INPUT_PROG     ";

void main() {
    printf("%s --------------------\n", name);
    while (1) {
        printf("%s - Type text: ", name);
        char buf[STDIN_BUFFER_SIZE];
        gets(buf);
        printf("%s - You typed: %s\n", name, buf);
    }
    printf("------------------------------\n");
}