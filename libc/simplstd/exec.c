#include <simplstd.h>

int32_t exec(uint32_t program_index) {
    static syscall_params params;

    params.param1 = program_index;

    return syscall(SYSCALL_SYS_EXEC, &params);
}