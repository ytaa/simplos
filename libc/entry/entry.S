#ifndef KERNEL_BUILD

.section .text
.global _start

_start:
    call main
    //_end_loop:
    //jmp _end_loop
    mov $2147484496, %eax
    jmp *%eax

#endif
