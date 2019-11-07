#include <stdio.h>
#include <sys/syscall.h>

int putchar(int ic) {
    static volatile char c = 0;
    c = ic;

    static volatile int32_t syscall_result = 5;
    asm volatile(
        "pusha\n\t"
        "movl %1, %%eax\n\t"
        "movl %2, %%ebx\n\t"
        "movl %3, %%ecx\n\t"
        "movl %4, %%edx\n\t"
        "int $0x80\n\t"
        "movl %%eax, %0\n\t"
        "popa"
        : "=rm"(syscall_result)
        : "rm"(SYSCALL_SYS_WRITE), "rm"(0), "rm"(&c), "rm"(1));

    return syscall_result;
}
