#include <simplstd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

char *gets(char *s) {
    static syscall_params params;
    static volatile bool is_blocked = false;
    static char buffer[STDIN_BUFFER_SIZE];
    char *sc = s;

    if (s) {
        params.param1 = STDIN;
        params.param2 = (uint32_t)buffer;
        params.param4 = (uint32_t)&is_blocked;

        (void)syscall(SYSCALL_SYS_READ, &params);

        while (is_blocked)
            ;

        for (char *c = buffer; *c != 0; c++, sc++) {
            *sc = *c;
        }
        *sc = 0;
    }

    return s;
}