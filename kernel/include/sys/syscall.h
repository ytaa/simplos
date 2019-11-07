#ifndef KERNEL_SYSCALL
#define KERNEL_SYSCALL

#include <stdint.h>

//syscall opcodes
#define SYSCALL_SYS_WRITE 1

//syscall return values
#define SYSCALL_STATUS_SUCCESS 0
#define SYSCALL_STATUS_INVALID_PARAM -2
#define SYSCALL_STATUS_INVALID_OPCODE -3

typedef struct {
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint32_t param4;
    uint32_t param5;
} syscall_params;

int32_t syscall(uint32_t opcode, syscall_params *params);
void syscall_irq_handler();

#endif