#ifndef _KERNEL_SCHEDULER
#define _KERNEL_SCHEDULER

#include <elf/elf32_loader.h>
#include <stdbool.h>
#include <stdint.h>

#define SCH_MAX_PROCESS_PAGE_TABLES 10
#define SCH_MAX_LOADED_PROCESS 50
#define SCH_MAX_PROCESS_SPACE_SIZE SCH_MAX_PROCESS_PAGE_TABLES *PG_PAGE_TABLE_SIZE

#define SCH_PROCESSES_MEMMORY_OFFSET PG_PAGE_TABLE_SIZE

#define SCH_KERNEL_STACK_SIZE 1024

//return status codes
#define SCH_STATUS_INVALID_FORMAT -2
#define SCH_STATUS_NO_SPACE -3

typedef enum {
    SCH_PSTATE_TERMINATED = 0,
    SCH_PSTATE_NEW,
    SCH_PSTATE_ACTIVE,
    SCH_PSTATE_SLEEPING,
    SCH_PSTATE_WAIT_INPUT,
    SCH_PSTATE_WAIT_CHILDREN
} sch_process_state;

typedef struct {
    void *data;
    uint32_t size;
} sch_elf_binary;

typedef struct {
    uint32_t kernel_stack_pointer;                //esp0 - expected offset 0
    uint32_t *page_directory;                     //cr3 - expected offset 4
    uint32_t stack_pointer;                       //esp - expected offset 8
    elf32_info elf_info;                          //process elf info - expected offset 12
    uint8_t kernel_stack[SCH_KERNEL_STACK_SIZE];  //process kernel stack - expected offset 20
    uint32_t pid;                                 //unique process identifier representing processes memmory slot used by the process
    sch_process_state state;                      //current process state
    uint32_t remaining_sleep_ms;                  //ms counter used during sleep state
    uint32_t child_processes_count;               //number of child process
    uint32_t parent_pid;                          //parent process identifier
    bool *is_blocked;                             //process space pointer to variable blocking process on blocking states
    uint8_t *input_buffer;                        //process space pointer to input buffer
} sch_process_control_block;

typedef struct {
    bool is_used;
    uint32_t start_paddr;
    sch_process_control_block pcb;
    uint32_t loaded_binary_idx;
} sch_memmory_slot;

extern sch_process_control_block *sch_current_pcb;

extern sch_process_control_block *sch_next_pcb;

extern uint32_t sch_request_program_index;

extern sch_process_control_block idle_pcb;

extern void sch_user_request_exec();

extern void sch_wakeup_process();

void sch_init();

void sch_run();

void sch_sleep_current_process(uint32_t sleep_ms, bool *is_sleeping);

void sch_wake_up_current_process();

void sch_input_block_current_process(uint8_t *input_buffer, bool *is_blocked);

void sch_wait_children_block_current_process(bool *is_blocked);

void sch_flush_input(const uint8_t *data, const uint32_t size);

kernel_return_t sch_request_exec();

void sch_request_exec_syscall(uint32_t program_index);

void sch_terminate_process();

void sch_schedule();

#endif