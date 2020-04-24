.text
.globl pg_load_page_directory
.extern pg_target_page_directory

pg_load_page_directory:
movl pg_target_page_directory, %eax
movl %eax, %cr3
ret

.text
.globl pg_enable_paging
pg_enable_paging:
push %ebp
mov %esp, %ebp
mov %cr0, %eax
or $0x80000000, %eax
mov %eax, %cr0
mov %ebp, %esp
pop %ebp
ret
