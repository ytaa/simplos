#ifndef KERNEL_BUILD

.section .text
.global _start

_start:
    call main

    mov $1, %eax
    int $0x80

    //mov $2147484496, %eax
    //jmp *%eax

    _end_loop:
    jmp _end_loop

#endif
