//WARNING: IRQs has to be disabled

.global sch_switch_process
.global sch_run_very_first_process
.global sch_switch_new_process
.global sch_user_request_exec
 
.extern sch_current_pcb
.extern sch_next_pcb
.extern sch_kernel_page_directory
.extern sch_request_exec
.extern sch_common_kernel_stack

.extern sbuf1
.extern sbuf2
.global stack_test1

sch_switch_process:
    //Save previous task's state
 
    //push registers to previous process stack
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esi
    push %edi
    push %ebp
 
    movl sch_current_pcb, %edi  //load address of previous process PCB
    movl %esp, (%edi)           //save ESP for previous process's stack in the PCB
 
    //Load next task's state
 
    movl sch_next_pcb, %esi     //load address of next process PCB
    movl %esi, sch_current_pcb   //set next PCB as current PCB
    
    movl (%esi), %esp          //load esp for next process from PCB
    movl 4(%esi), %eax         //load address of page directory for next process from PCB - offset 4 in PCB
    //mov ebx,[esi+TCB.ESP0]        ;ebx = address for the top of the next task's kernel stack
    //mov [TSS.ESP0],ebx            ;Adjust the ESP0 field in the TSS (used by CPU for for CPL=3 -> CPL=0 privilege level changes)
    //mov ecx,cr3                   ;ecx = previous task's virtual address space
 
    //cmp eax,ecx                   ;Does the virtual address space need to being changed?
    //je .doneVAS                   ; no, virtual address space is the same, so don't reload it and cause TLB flushes
    movl %eax, %cr3                   //load the next process virtual address space
//.doneVAS:

    //pop registers from next process stack
    pop %ebp
    pop %edi
    pop %esi
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax

    ret                           //load next process EIP from its stack

sch_switch_new_process:
    //Save previous task's state

    //push registers to previous process stack
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esi
    push %edi
    push %ebp
 
    movl sch_current_pcb, %edi  //load address of previous process PCB
    movl %esp, (%edi)           //save ESP for previous process's stack in the PCB
  
    //Load next task's state
 
    movl sch_next_pcb, %esi     //load address of next process PCB
    movl %esi, sch_current_pcb  //set next PCB as current PCB

    movl 8(%esi), %esp           //load esp for next process from PCB

    movl 4(%esi), %eax          //load address of page directory for next process from PCB - offset 4 in PCB

    movl %eax, %cr3             //load the next process virtual address space

    movl 16(%esi), %eax        //load entry address of new task



    //prepare stack for iret
    push %esp
    push $0x0000 // ds - ring0
    push $0x0000 // ss
    push $512 //eflags?
    push $8 //user code segment - ring 0
    push %eax //entry address

    //send EOI
    mov $0x20, %al
    outb %al, $0x20

    //return to new process entry
    sti
    iret 

    

sch_run_very_first_process:
    movl sch_current_pcb, %edi //load address of first process PCB
    movl (%edi), %esp          //load esp for first process from PCB
    movl 4(%edi), %eax         //load address of page directory for first process from PCB - offset 4 in PCB
    movl %eax, %cr3            //load the first process virtual address space
    movl 12(%edi), %eax        //load entry address of new task
    jmp *%eax                  //jump to new task entry


sch_user_request_exec:
    pusha

    //save current esp in pcb
    movl sch_current_pcb, %edi  //load address of current process PCB
    movl %esp, (%edi)           //save ESP for current process's stack in the PCB

    movl sch_common_kernel_stack, %esp //load common kernel stack ptr

    movl sch_kernel_page_directory, %eax

    movl %eax, %cr3 //load kernel page directory

    call sch_request_exec

    movl sch_current_pcb, %edi  //load address of current process PCB
    movl (%edi), %esp           //load ESP for current process's stack form the PCB

    movl 4(%edi), %eax         //load address of page directory for first process from PCB - offset 4 in PCB
    movl %eax, %cr3            //load the first process virtual address space

    popa
    ret

.extern print_stack
stack_test1:
    movl sbuf1, %ebx

    mov (%esp), %eax
    mov %eax, (%ebx)
    
    mov 4(%esp), %eax
    mov %eax, 4(%ebx)

    mov 8(%esp), %eax
    mov %eax, 8(%ebx)

    movl 12(%esp), %eax
    movl %eax, 12(%ebx)
    
    mov 16(%esp), %eax
    mov %eax, 16(%ebx)

    mov 20(%esp), %eax
    mov %eax, 20(%ebx)

    movl 24(%esp), %eax
    movl %eax, 24(%ebx)

    movl 28(%esp), %eax
    movl %eax, 28(%ebx)

   call print_stack 

    ret

stack_test2:
    movl sbuf2, %ebx
    
    mov (%esp), %eax
    mov %eax, (%ebx)

    mov 4(%esp), %eax
    mov %eax, 4(%ebx)

    mov 8(%esp), %eax
    mov %eax, 8(%ebx)

    movl 12(%esp), %eax
    movl %eax, 12(%ebx)
    
    mov 16(%esp), %eax
    mov %eax, 16(%ebx)

    mov 20(%esp), %eax
    mov %eax, 20(%ebx)

    movl 24(%esp), %eax
    movl %eax, 24(%ebx)

    movl 28(%esp), %eax
    movl %eax, 28(%ebx)

    ret

