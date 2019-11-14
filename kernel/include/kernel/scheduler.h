#ifndef _KERNEL_SCHEDULER
#define _KERNEL_SCHEDULER

#include <stdbool.h>
#include <stdint.h>

#define SCH_MAX_STORED_BINARIES 20
#define SCH_MAX_PROCESS_PAGE_TABLES 10
#define SCH_MAX_LOADED_PROCESS 50

typedef struct {
    void *data;
    uint32_t size;
} sch_elf_binary;

typedef struct {
    uint32_t pid;  //unique process identifier representing processes memmory slot used by the process
} sch_loaded_process;

typedef struct {
    bool is_used;
} sch_memmory_slot;

void sch_init();

void sch_request_exec(uint32_t program_index);

#endif