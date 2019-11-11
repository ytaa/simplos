#include <kernel/kernel_syscall.h>
#include <kernel/tty.h>
#include <stdio.h>

static int32_t sys_write(uint32_t fd, uint8_t* data, uint32_t size) {
    int32_t status = SYSCALL_STATUS_SUCCESS;

    if (fd == STDOUT) {
        tty_write((const char*)data, size);
    } else {
        status = SYSCALL_INVALID_FD;
    }

    return status;
}

int32_t kernel_syscall(uint32_t opcode, volatile syscall_params* prs) {
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

void kernel_syscall_irq_handler() {
    static volatile uint32_t opcode = 0;
    static volatile syscall_params params;

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
        : "r"(kernel_syscall(opcode, &params)));
}