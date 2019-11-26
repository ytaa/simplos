.global kernel_syscall_read_params

.extern kernel_syscall_opcode
.extern kernel_syscall_params_ptr

kernel_syscall_read_params:
    movl %eax, kernel_syscall_opcode

    movl kernel_syscall_params_ptr, %eax
    movl %ebx, (%eax)
    movl %ecx, 4(%eax)
    movl %edx, 8(%eax)
    movl %esi, 12(%eax)
    movl %edi, 16(%eax)
ret
