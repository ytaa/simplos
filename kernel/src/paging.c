#include <stdint.h>
#include <string.h>

#include <stdio.h>
#include "kernel/paging.h"

//functions from paging_setup.s
extern void pg_load_page_directory(uint32_t *);
extern void pg_enable_paging();

static uint32_t kernel_page_directory[PG_PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
static uint32_t kernel_page_table_address_space_start[PG_PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));
static uint32_t kernel_higher_half_page_tables[PG_KERNEL_HIGHER_HALF_PAGE_TABLE_AMOUNT][PG_PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

static void setup_identity_page_table(uint32_t *first_pte, uint32_t from, uint64_t size) {
    from = from & 0xfffff000;  // discard bits we don't want
    for (; size > 0; from += PG_ALLIGN, size -= PG_ALLIGN, first_pte++) {
        *first_pte = from | 1;  // mark page present.
    }
}

static void clear_directory(uint32_t *page_directory) {
    //set each entry to not present
    for (uint16_t i = 0; i < 1024; i++) {
        //Supervisor: kernel-mode
        //Write Enabled: read-write
        //Not Present: true
        page_directory[i] = 0x00000002;
    }
}

static void put_table_to_directory(uint32_t *page_directory, uint32_t *page_table, uint32_t table_index) {
    page_directory[table_index] = ((uint32_t)page_table) | 3;
}

static void setup_kernel_identity_paging() {
    //setup first 4MB to identity paging kernel space - reserved for among others VGA buffer
    setup_identity_page_table(kernel_page_table_address_space_start, 0, PG_ALLIGN * PG_PAGE_TABLE_ENTRIES);
    put_table_to_directory(kernel_page_directory, kernel_page_table_address_space_start, 0);

    //setup identity paging kernel space where kernel is loaded (higher half kernel)
    for (uint32_t page_table_index = 0; page_table_index < PG_KERNEL_HIGHER_HALF_PAGE_TABLE_AMOUNT; page_table_index++) {
        setup_identity_page_table(kernel_higher_half_page_tables[page_table_index],
                                  (PG_PAGE_TABLE_HALF_INDEX + page_table_index) * PG_PAGE_TABLE_SIZE,
                                  PG_ALLIGN * PG_PAGE_TABLE_ENTRIES);
        put_table_to_directory(kernel_page_directory, kernel_higher_half_page_tables[page_table_index],
                               PG_PAGE_TABLE_HALF_INDEX + page_table_index);
    }
}

void pg_init_paging() {
    clear_directory(kernel_page_directory);

    setup_kernel_identity_paging();

    pg_load_page_directory(kernel_page_directory);
    pg_enable_paging();
}
