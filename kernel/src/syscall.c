#include <kernel/tty.h>
#include <stdio.h>
#include <sys/syscall.h>

static int32_t sys_write(uint32_t fd, uint8_t* data, uint32_t size) {
    int32_t status = SYSCALL_STATUS_SUCCESS;
    (void)fd;
    for (uint32_t char_index = 0; char_index < size; char_index++) {
        tty_put_char(data[char_index]);
    }
    return status;
}

int32_t syscall(uint32_t opcode, syscall_params* prs) {
    int32_t status = SYSCALL_STATUS_INVALID_PARAM;
    if (prs) {
        status = SYSCALL_STATUS_INVALID_OPCODE;
        switch (opcode) {
            case SYSCALL_SYS_WRITE:
                status = sys_write(prs->param1, (uint8_t*)(prs->param2), prs->param3);
                break;
            default:
                break;
        }
    }
    return status;
}

void syscall_irq_handler() {
    uint32_t opcode = 0;
    syscall_params params;

    asm volatile(
        "movl %%eax, %0\n\t"
        "movl %%ebx, %1\n\t"
        "movl %%ecx, %2\n\t"
        "movl %%edx, %3\n\t"
        "movl %%esi, %4\n\t"
        "movl %%edi, %5"
        : "=rm"(opcode), "=rm"(params.param1), "=rm"(params.param2), "=rm"(params.param3), "=rm"(params.param4), "=rm"(params.param5)
        :);

    asm volatile(
        "movl %0, %%eax"
        :
        : "r"(syscall(opcode, &params)));
}