#include <elf/elf32_loader.h>
#include <kernel/paging.h>
#include <kernel/scheduler.h>
#include <stdio.h>

//precompiled elf binaries
#include <elf/elf_test.h>

static sch_elf_binary binaries[SCH_MAX_STORED_BINARIES];

static sch_memmory_slot memmory_slots[SCH_MAX_LOADED_PROCESS];

void sch_init() {
    //setup binaries
    binaries[0].data = test_elf_buffer;
    binaries[0].size = test_elf_size;

    //setup memmory slots
    for (uint32_t slot_id = 0; slot_id < SCH_MAX_LOADED_PROCESS; slot_id++) {
        memmory_slots[slot_id].is_used = false;
        memmory_slots[slot_id].start_paddr = SCH_PROCESSES_MEMMORY_OFFSET + SCH_MAX_PROCESS_SPACE_SIZE * slot_id;
    }
}

void sch_request_exec(uint32_t program_index) {
    uint32_t pid = 1;
    elf32_info elf_info;

    elf32_load_exec(binaries[program_index].data, memmory_slots[pid].start_paddr, &elf_info);

    printf("Actual elf base vaddr: %u\n", elf_info.base_vaddr);
    printf("Asigned paddr: %u in slot %u\n", memmory_slots[pid].start_paddr, pid);

    pg_update_process_page_direcotry(pid, elf_info.base_vaddr);
    pg_load_process_page_directory(pid);
    printf("\nSwitched to process page directory\n");

    printf("Jumping to vaddr %u...\n", elf_info.entry_vaddr);
    asm volatile(
        "jmp *%0"
        :
        : "rm"(elf_info.entry_vaddr));
}