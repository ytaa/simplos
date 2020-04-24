#include <simplstd.h>

int32_t exit() {
    static syscall_params params;

    syscall(SYSCALL_SYS_EXIT, &params);

    while (1)
        ;
}