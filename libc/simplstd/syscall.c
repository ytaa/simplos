#include <simplstd.h>

extern void syscall_call();

volatile uint32_t syscall_opcode = 0;
volatile uint32_t syscall_param1 = 0;
volatile uint32_t syscall_param2 = 0;
volatile uint32_t syscall_param3 = 0;
volatile uint32_t syscall_param4 = 0;
volatile uint32_t syscall_param5 = 0;
volatile int32_t syscall_result = 0;

int32_t syscall(uint32_t a_opcode, syscall_params *a_params) {
    syscall_opcode = a_opcode;
    syscall_param1 = a_params->param1;
    syscall_param2 = a_params->param2;
    syscall_param3 = a_params->param3;
    syscall_param4 = a_params->param4;
    syscall_param5 = a_params->param5;

    syscall_call();

    return syscall_result;
}