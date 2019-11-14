#include "kernel/paging.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//functions from paging_setup.s
extern void pg_load_page_directory(uint32_t *);
extern void pg_enable_paging();

static uint32_t kernel_page_directory[PG_PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
static uint32_t kernel_lower_half_page_tables[PG_PAGE_DIRECTORY_ENTRIES / 2][PG_PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));
static uint32_t kernel_higher_half_page_tables[PG_KERNEL_HIGHER_HALF_PAGE_TABLE_AMOUNT][PG_PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

static void setup_identity_page_table(uint32_t *first_pte, uint32_t from, uint64_t size, bool is_user_space) {
    from = from & 0xfffff000;  // discard bits we don't want
    uint32_t flags = 3;
    if (is_user_space) {
        flags = 7;
    }
    for (; size > 0; from += PG_ALLIGN, size -= PG_ALLIGN, first_pte++) {
        *first_pte = from | flags;  // mark page present.
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

static void put_table_to_directory(uint32_t *page_directory, uint32_t *page_table, uint32_t table_index, bool is_user_space) {
    if (is_user_space) {
        page_directory[table_index] = ((uint32_t)page_table) | 7;
    } else {
        page_directory[table_index] = ((uint32_t)page_table) | 3;
    }
}

static void setup_kernel_identity_paging() {
    //setup identity paging user space for lowe half of address space
    for (uint32_t page_table_index = 0; page_table_index < PG_PAGE_DIRECTORY_ENTRIES / 2; page_table_index++) {
        setup_identity_page_table(kernel_lower_half_page_tables[page_table_index],
                                  page_table_index * PG_PAGE_TABLE_SIZE,
                                  PG_ALLIGN * PG_PAGE_TABLE_ENTRIES, true);
        put_table_to_directory(kernel_page_directory, kernel_lower_half_page_tables[page_table_index],
                               page_table_index, true);
    }

    //setup identity paging kernel space where kernel is loaded (higher half kernel)
    for (uint32_t page_table_index = 0; page_table_index < PG_KERNEL_HIGHER_HALF_PAGE_TABLE_AMOUNT; page_table_index++) {
        setup_identity_page_table(kernel_higher_half_page_tables[page_table_index],
                                  (PG_PAGE_TABLE_HALF_INDEX + page_table_index) * PG_PAGE_TABLE_SIZE,
                                  PG_ALLIGN * PG_PAGE_TABLE_ENTRIES, false);
        put_table_to_directory(kernel_page_directory, kernel_higher_half_page_tables[page_table_index],
                               PG_PAGE_TABLE_HALF_INDEX + page_table_index, false);
    }
}

void pg_init_paging() {
    clear_directory(kernel_page_directory);

    setup_kernel_identity_paging();

    pg_load_page_directory(kernel_page_directory);
    pg_enable_paging();
}
