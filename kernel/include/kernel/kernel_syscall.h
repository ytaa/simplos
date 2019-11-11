#ifndef KERNEL_SYSCALL
#define KERNEL_SYSCALL

#include <simplstd.h>
#include <stdint.h>

void kernel_syscall_irq_handler();
int32_t kernel_syscall(uint32_t opcode, volatile syscall_params *params);

#endif