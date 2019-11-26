.extern syscall_opcode
.extern syscall_param1
.extern syscall_param2
.extern syscall_param3
.extern syscall_param4
.extern syscall_param5
.extern syscall_result

.global syscall_call
syscall_call:
    pusha

    movl syscall_opcode, %eax
    movl syscall_param1, %ebx
    movl syscall_param2, %ecx
    movl syscall_param3, %edx
    movl syscall_param4, %esi
    movl syscall_param5, %edi

    int $0x80
    movl %eax, syscall_result

    popa
ret
