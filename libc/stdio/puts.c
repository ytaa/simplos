#include <simplstd.h>
#include <stdio.h>
#include <string.h>

int puts(const char* string) {
    static syscall_params params;
    params.param1 = STDOUT;
    params.param2 = (uint32_t)string;
    params.param3 = strlen(string);

    return syscall(SYSCALL_SYS_WRITE, &params);
}
