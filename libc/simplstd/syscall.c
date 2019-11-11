#include <simplstd.h>

int32_t syscall(uint32_t a_opcode, syscall_params *a_params) {
    static volatile int32_t syscall_result = 0;
    static volatile uint32_t opcode = 0;
    static volatile uint32_t param1 = 0;
    static volatile uint32_t param2 = 0;
    static volatile uint32_t param3 = 0;
    static volatile uint32_t param4 = 0;
    static volatile uint32_t param5 = 0;

    opcode = a_opcode;
    param1 = a_params->param1;
    param2 = a_params->param2;
    param3 = a_params->param3;
    param4 = a_params->param4;
    param5 = a_params->param5;

    asm volatile(
        "pusha\n\t"
        "movl %1, %%eax\n\t"
        "movl %2, %%ebx\n\t"
        "movl %3, %%ecx\n\t"
        "movl %4, %%edx\n\t"
        "movl %5, %%esi\n\t"
        "movl %6, %%edi\n\t"
        "int $0x80\n\t"
        "movl %%eax, %0\n\t"
        "popa"
        : "=rm"(syscall_result)
        : "rm"(opcode), "rm"(param1), "rm"(param2),
          "rm"(param3), "rm"(param4), "rm"(param5));
    return syscall_result;
}