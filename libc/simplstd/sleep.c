#include <simplstd.h>
#include <stdbool.h>
#include <stdio.h>

int32_t sleep(uint32_t time_ms) {
    static syscall_params params;
    static volatile bool is_sleep = false;

    params.param1 = time_ms;
    params.param2 = (uint32_t)&is_sleep;

    int32_t status = syscall(SYSCALL_SYS_MSSLEEP, &params);
    while (is_sleep)
        ;

    return status;
}