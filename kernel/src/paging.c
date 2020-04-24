#include <kernel/paging.h>
#include <stdio.h>
#include <string.h>

uint32_t pg_kernel_page_directory[PG_PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
uint32_t pg_kernel_lower_half_page_tables[PG_PAGE_DIRECTORY_ENTRIES / 2][PG_PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));
uint32_t pg_kernel_higher_half_page_tables[PG_KERNEL_HIGHER_HALF_PAGE_TABLE_AMOUNT][PG_PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

uint32_t pg_processes_page_directories[SCH_MAX_LOADED_PROCESS][PG_PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
uint32_t pg_processes_page_tables[SCH_MAX_LOADED_PROCESS][SCH_MAX_PROCESS_PAGE_TABLES][PG_PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

uint32_t *pg_target_page_directory = 0u;

static void clear_directory(uint32_t *page_directory) {
    //set each entry to not present
    for (uint16_t i = 0; i < PG_PAGE_DIRECTORY_ENTRIES; i++) {
        //Supervisor: kernel-mode
        //Write Enabled: read-write
        //Not Present: true
        page_directory[i] = 0x00000002;
    }
}

static uint32_t vaddr_to_page_directory_index(uint32_t vaddr) {
    return vaddr / PG_PAGE_TABLE_SIZE;
}

static void setup_continous_page_table(uint32_t *first_pte, uint32_t from, uint64_t size, bool is_user_space) {
    from = from & 0xfffff000;  // discard bits we don't want
    uint32_t flags = 3;
    if (is_user_space) {
        flags = 7;
    }
    for (; size > 0; from += PG_ALLIGN, size -= PG_ALLIGN, first_pte++) {
        *first_pte = from | flags;  // mark page present.
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
        setup_continous_page_table(pg_kernel_lower_half_page_tables[page_table_index],
                                   page_table_index * PG_PAGE_TABLE_SIZE,
                                   PG_ALLIGN * PG_PAGE_TABLE_ENTRIES, false);
        put_table_to_directory(pg_kernel_page_directory, pg_kernel_lower_half_page_tables[page_table_index],
                               page_table_index, false);
    }

    //setup identity paging kernel space where kernel is loaded (higher half kernel)
    for (uint32_t page_table_index = 0; page_table_index < PG_KERNEL_HIGHER_HALF_PAGE_TABLE_AMOUNT; page_table_index++) {
        setup_continous_page_table(pg_kernel_higher_half_page_tables[page_table_index],
                                   (PG_PAGE_TABLE_HALF_INDEX + page_table_index) * PG_PAGE_TABLE_SIZE,
                                   PG_ALLIGN * PG_PAGE_TABLE_ENTRIES, false);
        put_table_to_directory(pg_kernel_page_directory, pg_kernel_higher_half_page_tables[page_table_index],
                               PG_PAGE_TABLE_HALF_INDEX + page_table_index, false);
    }
}

static void setup_processes_paging() {
    //setup processes page tables
    for (uint32_t pid = 0; pid < SCH_MAX_LOADED_PROCESS; pid++) {
        for (uint32_t table_idx = 0; table_idx < SCH_MAX_PROCESS_PAGE_TABLES; table_idx++) {
            setup_continous_page_table(pg_processes_page_tables[pid][table_idx],
                                       SCH_PROCESSES_MEMMORY_OFFSET - PG_PROCESS_MEMMORY_OFFSET_FIX +  //offset for first 4 mb reserved for kernel
                                           SCH_MAX_PROCESS_SPACE_SIZE * pid + table_idx * PG_PAGE_TABLE_SIZE,
                                       PG_PAGE_TABLE_SIZE,
                                       true);  // user space
        }
    }

    //setup processes page directories
    for (uint32_t pid = 0; pid < SCH_MAX_LOADED_PROCESS; pid++) {
        //set first half entries to not present user-mode
        for (uint16_t page_table_index = 0; page_table_index < PG_PAGE_DIRECTORY_ENTRIES / 2; page_table_index++) {
            pg_processes_page_directories[pid][page_table_index] = 0x00000006;
        }

        //set second half entries to kernel page tables
        for (uint32_t page_table_index = 0; page_table_index < PG_KERNEL_HIGHER_HALF_PAGE_TABLE_AMOUNT; page_table_index++) {
            put_table_to_directory(pg_processes_page_directories[pid], pg_kernel_higher_half_page_tables[page_table_index],
                                   PG_PAGE_TABLE_HALF_INDEX + page_table_index, false);
        }

        //set first reserved entry to kernel space
        put_table_to_directory(pg_processes_page_directories[pid], pg_kernel_lower_half_page_tables[0], 0, false);
    }
}

void pg_update_process_page_direcotry(uint32_t pid, uint32_t base_vaddr) {
    uint32_t page_directory_index = vaddr_to_page_directory_index(base_vaddr);

    //printf("Calculated page directory index: %u starting at vaddr %u\n", page_directory_index, page_directory_index * PG_PAGE_TABLE_SIZE);
    for (uint32_t page_table_index = 0; page_table_index < SCH_MAX_PROCESS_PAGE_TABLES; page_table_index++) {
        put_table_to_directory(pg_processes_page_directories[pid],
                               pg_processes_page_tables[pid][page_table_index],
                               page_directory_index + page_table_index,
                               true);  //user space
    }
    //printf("Expected process vaddr: %u to %u\n", page_directory_index * PG_PAGE_TABLE_SIZE, (page_directory_index + SCH_MAX_PROCESS_PAGE_TABLES) * PG_PAGE_TABLE_SIZE);
    //printf("Expected process paddr: %u\n", ((uint32_t *)(pg_processes_page_directories[pid][page_directory_index] & 0xfffff000))[0]);
}

void pg_load_kernel_page_directory() {
    pg_target_page_directory = pg_kernel_page_directory;
    pg_load_page_directory();
}

void pg_load_process_page_directory(uint32_t pid) {
    pg_target_page_directory = pg_processes_page_directories[pid];
    pg_load_page_directory();
}

void pg_init_paging() {
    clear_directory(pg_kernel_page_directory);

    setup_kernel_identity_paging();

    setup_processes_paging();

    pg_load_kernel_page_directory();
    pg_enable_paging();
}
