ENTRY(_start)

SECTIONS
{
	//. = 4M;
	. = 0x80000000; 
	//. = 0xC0000000;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.gdt)
		*(.isr)
		*(.text)
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
