#include <elf/elf32_loader.h>
#include <kernel/paging.h>
#include <kernel/scheduler.h>

//precompiled elf binaries
#include <elf/elf_test.h>

static uint32_t process_page_tables[SCH_MAX_LOADED_PROCESS][SCH_MAX_PROCESS_PAGE_TABLES][PG_PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

static sch_elf_binary binaries[SCH_MAX_STORED_BINARIES];

void sch_init() {
    binaries[0].data = test_elf_buffer;
    binaries[0].size = test_elf_size;
}

void sch_request_exec(uint32_t program_index) {
}