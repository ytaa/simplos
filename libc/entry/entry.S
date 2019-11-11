#ifndef KERNEL_BUILD

.section .text
.global _start

_start:
    call main
    _end_loop:
    jmp _end_loop

#endif
