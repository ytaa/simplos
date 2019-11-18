#ifndef PAGING_H
#define PAGING_H

#include <stdbool.h>
#include <stdint.h>

//functions from paging_setup.s
extern void pg_load_page_directory(uint32_t *);
extern void pg_enable_paging();

#define PG_PAGE_DIRECTORY_ENTRIES 1024u
#define PG_PAGE_TABLE_ENTRIES 1024u
#define PG_ALLIGN 4096u  //0x1000
#define PG_PAGE_TABLE_SIZE 0x400000u

#define PG_KERNEL_PADDR 0x80000000u
#define PG_KERNEL_HIGHER_HALF_PAGE_TABLE_AMOUNT 100u
#define PG_PAGE_TABLE_HALF_INDEX PG_PAGE_TABLE_ENTRIES / 2u

#define PG_PROCESS_MEMMORY_OFFSET_FIX 294912

void pg_init_paging();

void pg_update_process_page_direcotry(uint32_t pid, uint32_t base_vaddr);

void pg_load_process_page_directory(uint32_t pid);

void pg_load_kernel_page_directory();

#endif