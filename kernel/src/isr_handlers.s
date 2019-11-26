.section .isr
.global isr_idt_load
.global isr_irq0
.global isr_irq1
.global isr_irq_syscall

.extern isr_irq0_handler
.extern isr_irq1_handler
.extern isr_irq_syscall_handler

.extern sch_current_pcb
.extern sch_is_running
.extern irq_caller_esp

//.extern stack_test1
isr_idt_load:
    mov 4(%esp), %edx
    lidt (%edx)
    sti
    ret
isr_irq0:
    cli
    pusha

    cmp $0, sch_is_running
    jne isr_irq0_change_stack

    call isr_irq0_handler

    popa
    sti
    iret
isr_irq0_change_stack:
    movl sch_current_pcb, %eax
    movl %esp, 8(%eax)
    movl (%eax), %esp

    call isr_irq0_handler

    movl sch_current_pcb, %eax
    movl %esp, (%eax) 
    movl 8(%eax), %esp

    popa
    sti
    iret
isr_irq1:
    pusha
    call isr_irq1_handler
    popa
    iret
isr_irq_syscall:
    call isr_irq_syscall_handler
    //call stack_test1
    iret
