/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */
 
/* 
Declare a multiboot header
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
 
/*
allocating 16 KiB for kernel stack
*/
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
 
/*
linker script specifies _start as the entry point to the kernel and the
bootloader will jump to this position once the kernel has been loaded. 
*/
.section .text
.global _start
.type _start, @function
_start:
	/*
	stack grows downwards on x86 systems
	*/
	mov $stack_top, %esp

	/*
	Enter the high-level kernel.
	*/
	call kernel_main
 
	//Disable interrupts
	cli
	//Wait for next interrupt
1:	hlt
	//jump to hlt for infinite loop
	jmp 1b
 
/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start
