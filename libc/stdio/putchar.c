#include <simplstd.h>
#include <stdio.h>
#include <string.h>

int putchar(int ic) {
    static syscall_params params;
    static volatile char c = 0;
    c = ic;
    params.param1 = STDOUT;
    params.param2 = (uint32_t)&c;
    params.param3 = 1;

    return syscall(SYSCALL_SYS_WRITE, &params);
}
