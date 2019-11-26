#include <simplstd.h>
#include <stdbool.h>

int32_t wait() {
    static syscall_params params;
    static volatile bool is_blocked = false;

    params.param1 = (uint32_t)&is_blocked;

    int32_t status = syscall(SYSCALL_SYS_WAIT, &params);
    while (is_blocked)
        ;

    return status;
}