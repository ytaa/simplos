#include <kernel/kernel_utils.h>
#include <kernel/paging.h>
#include <kernel/pit/pit_timer.h>
#include <kernel/scheduler.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <string.h>

//precompiled elf binaries
#include <elf/elf_input.h>
#include <elf/elf_shell.h>
#include <elf/elf_test.h>
#include <elf/elf_test2.h>

#define INVALID_MEMMORY_SLOT -1
#define COMMON_KERNEL_STACK_SIZE 1024
#define SCHEDULE_TRIGGER_TICK_PASS 3
#define SCHEDULE_DELTA_MS PITT_MS_PER_CH0_TICK *(SCHEDULE_TRIGGER_TICK_PASS + 1)

//functions from process_switch.s
extern void sch_run_very_first_process();

extern void sch_switch_new_process();

extern void sch_switch_process();

//global variables
sch_process_control_block *sch_current_pcb;

sch_process_control_block *sch_next_pcb;

sch_process_control_block idle_pcb;

uint32_t sch_common_kernel_stack_buffer[COMMON_KERNEL_STACK_SIZE];

uint32_t sch_idle_stack_buffer[COMMON_KERNEL_STACK_SIZE];

uint32_t sch_common_kernel_stack;

uint32_t sch_request_program_index;

uint32_t *sch_kernel_page_directory;

bool sch_is_running;
bool is_idle;

//static variables
static sch_elf_binary binaries[SCH_MAX_STORED_BINARIES];

static sch_memmory_slot memmory_slots[SCH_MAX_LOADED_PROCESS];

static uint32_t current_slot_index;

static uint32_t loaded_process_num;

static uint32_t schedule_trigger_ticks;

void sch_idle_handler() {
    idle_pcb.state = SCH_PSTATE_ACTIVE;
    idle_pcb.stack_pointer = ((uint32_t) & (sch_idle_stack_buffer)) + sizeof(sch_idle_stack_buffer);
    idle_pcb.kernel_stack_pointer = ((uint32_t) & (idle_pcb.kernel_stack)) + sizeof(idle_pcb.kernel_stack);
    is_idle = true;
    sch_current_pcb = &idle_pcb;

    sch_is_running = true;
    kuts_printk("[SCH] Scheduler activated\n");
    while (1)  //kuts_printk("kernel idle\n");
        ;
}

//static functions

static int32_t get_unused_memmory_slot() {
    int32_t unused_slot_index = INVALID_MEMMORY_SLOT;

    for (uint32_t slot_index = 0; slot_index < SCH_MAX_LOADED_PROCESS; slot_index++) {
        if (memmory_slots[slot_index].is_used == false) {
            unused_slot_index = slot_index;
            break;
        }
    }

    return unused_slot_index;
}

static void handle_sleeping_processes() {
    for (uint32_t slot_index = 0; slot_index < SCH_MAX_LOADED_PROCESS; slot_index++) {
        if (memmory_slots[slot_index].is_used == true) {
            if (memmory_slots[slot_index].pcb.state == SCH_PSTATE_SLEEPING) {
                if (memmory_slots[slot_index].pcb.remaining_sleep_ms > SCHEDULE_DELTA_MS) {
                    memmory_slots[slot_index].pcb.remaining_sleep_ms -= SCHEDULE_DELTA_MS;
                } else {
                    memmory_slots[slot_index].pcb.remaining_sleep_ms = 0;
                    memmory_slots[slot_index].pcb.state = SCH_PSTATE_ACTIVE;
                    pg_load_process_page_directory(slot_index);
                    *(memmory_slots[slot_index].pcb.is_blocked) = false;
                    pg_load_process_page_directory(current_slot_index);
                }
            } else if (memmory_slots[slot_index].pcb.state == SCH_PSTATE_WAIT_CHILDREN) {
                if (memmory_slots[slot_index].pcb.child_processes_count == 0) {
                    memmory_slots[slot_index].pcb.state = SCH_PSTATE_ACTIVE;
                    pg_load_process_page_directory(slot_index);
                    *(memmory_slots[slot_index].pcb.is_blocked) = false;
                    pg_load_process_page_directory(current_slot_index);
                }
            }
        }
    }
}

//global functions

void sch_init() {
    //init common kernel stack
    sch_common_kernel_stack = (uint32_t)(((uint32_t *)sch_common_kernel_stack_buffer) + COMMON_KERNEL_STACK_SIZE / 2);

    //setup pcb for idle state
    idle_pcb.page_directory = pg_kernel_page_directory;

    //adaptation for asm code
    sch_kernel_page_directory = pg_kernel_page_directory;

    //setup binaries
    binaries[0].data = shell_elf_buffer;
    binaries[0].size = shell_elf_size;

    binaries[1].data = test2_elf_buffer;
    binaries[1].size = test2_elf_size;

    binaries[2].data = input_elf_buffer;
    binaries[2].size = input_elf_size;

    binaries[3].data = test_elf_buffer;
    binaries[3].size = test_elf_size;

    //setup memmory slots
    for (uint32_t slot_id = 0; slot_id < SCH_MAX_LOADED_PROCESS; slot_id++) {
        memmory_slots[slot_id].is_used = false;
        memmory_slots[slot_id].start_paddr = SCH_PROCESSES_MEMMORY_OFFSET + SCH_MAX_PROCESS_SPACE_SIZE * slot_id;
    }

    //reset loaded processes
    current_slot_index = 0;
    for (uint32_t slot_index = 0; slot_index < SCH_MAX_LOADED_PROCESS; slot_index++) {
        memmory_slots[slot_index].is_used = false;
    }
}

void sch_run() {
    sch_idle_handler();
}

extern uint32_t *sbuf1;
extern uint32_t *sbuf2;

void print_stack() {
    for (int i = 0; i < 10; i++) {
        kuts_printk("%u\n", sbuf1[i]);
    }
    while (1)
        ;
}

void sch_request_exec_syscall(uint32_t program_index) {
    sch_request_program_index = program_index;
    sch_user_request_exec();
}

//caller is expected to load kernel page directory
kernel_return_t sch_request_exec() {
    kernel_return_t status = KERNEL_STATUS_SUCCESS;

    uint32_t program_index = sch_request_program_index;

    int32_t pid = get_unused_memmory_slot();

    if (pid == INVALID_MEMMORY_SLOT) {
        return SCH_STATUS_NO_SPACE;
    }

    elf32_info elf_info;

    status = elf32_load_exec(binaries[program_index].data, memmory_slots[pid].start_paddr, &elf_info);
    if (status != KERNEL_STATUS_SUCCESS) {
        return SCH_STATUS_INVALID_FORMAT;
    }

    memmory_slots[pid].is_used = true;

    //setup new proccess pcb
    bzero(&(memmory_slots[pid].pcb), sizeof(memmory_slots[pid].pcb));
    memmory_slots[pid].pcb.pid = pid;
    memmory_slots[pid].pcb.elf_info = elf_info;
    memmory_slots[pid].pcb.stack_pointer = elf_info.base_vaddr + SCH_MAX_PROCESS_SPACE_SIZE - PG_PAGE_TABLE_SIZE;
    memmory_slots[pid].pcb.kernel_stack_pointer = ((uint32_t) & (memmory_slots[pid].pcb.kernel_stack)) + sizeof(memmory_slots[pid].pcb.kernel_stack);
    kuts_printk("[SCH] basev: %u, max space %u, stack: %u\n", elf_info.base_vaddr, SCH_MAX_PROCESS_SPACE_SIZE, memmory_slots[pid].pcb.stack_pointer);
    memmory_slots[pid].pcb.page_directory = pg_processes_page_directories[pid];
    memmory_slots[pid].pcb.state = SCH_PSTATE_NEW;

    if (sch_is_running == true) {
        memmory_slots[pid].pcb.parent_pid = current_slot_index;
        memmory_slots[current_slot_index].pcb.child_processes_count++;
    } else {
        memmory_slots[pid].pcb.parent_pid = pid;
    }

    pg_update_process_page_direcotry(pid, elf_info.base_vaddr);

    //printf("Actual elf base vaddr: %u\n", elf_info.base_vaddr);
    //printf("Actual elf entry vaddr: %u\n", elf_info.entry_vaddr);
    kuts_printk("[SCH] New process loaded - asigned paddr: %u in slot %u\n", memmory_slots[pid].start_paddr, pid);

    loaded_process_num++;

    return status;
}

void sch_sleep_current_process(uint32_t sleep_ms, bool *is_sleeping) {
    sch_current_pcb->state = SCH_PSTATE_SLEEPING;
    sch_current_pcb->remaining_sleep_ms = sleep_ms;
    sch_current_pcb->is_blocked = is_sleeping;
    *is_sleeping = true;
}

void sch_input_block_current_process(uint8_t *input_buffer, bool *is_blocked) {
    sch_current_pcb->state = SCH_PSTATE_WAIT_INPUT;
    sch_current_pcb->input_buffer = input_buffer;
    sch_current_pcb->is_blocked = is_blocked;
    *is_blocked = true;
}

void sch_wait_children_block_current_process(bool *is_blocked) {
    sch_current_pcb->state = SCH_PSTATE_WAIT_CHILDREN;
    sch_current_pcb->is_blocked = is_blocked;
    *is_blocked = true;
}

void sch_flush_input(const uint8_t *data, const uint32_t size) {
    bool pg_changed = false;
    for (uint32_t slot_index = 0; slot_index < SCH_MAX_LOADED_PROCESS; slot_index++) {
        if (memmory_slots[slot_index].is_used == true &&
            memmory_slots[slot_index].pcb.state == SCH_PSTATE_WAIT_INPUT) {
            pg_load_process_page_directory(slot_index);
            pg_changed = true;
            memcpy(memmory_slots[slot_index].pcb.input_buffer, data, size + 1);
            *(memmory_slots[slot_index].pcb.is_blocked) = false;
            memmory_slots[slot_index].pcb.state = SCH_PSTATE_ACTIVE;
        }
    }

    if (pg_changed) {
        if (is_idle) {
            pg_load_kernel_page_directory();
        } else {
            pg_load_process_page_directory(current_slot_index);
        }
    }
}

void sch_terminate_process() {
    //handle parent process
    if (memmory_slots[current_slot_index].pcb.parent_pid != current_slot_index) {
        sch_memmory_slot *parent = &(memmory_slots[memmory_slots[current_slot_index].pcb.parent_pid]);
        if (parent->pcb.child_processes_count <= 1) {
            parent->pcb.child_processes_count = 0;
            if (parent->pcb.state == SCH_PSTATE_TERMINATED) {
                parent->is_used = false;
            } else if (parent->pcb.state == SCH_PSTATE_WAIT_CHILDREN) {
                /*parent->pcb.state = SCH_PSTATE_ACTIVE;
                pg_load_process_page_directory(parent->pcb.pid);
                *(parent->pcb.is_blocked) = false;
                pg_load_process_page_directory(current_slot_index);*/
            }
        } else {
            memmory_slots[memmory_slots[current_slot_index].pcb.parent_pid].pcb.child_processes_count--;
        }
    }

    if (memmory_slots[current_slot_index].pcb.child_processes_count == 0) {
        memmory_slots[current_slot_index].is_used = false;
    }

    memmory_slots[current_slot_index].pcb.state = SCH_PSTATE_TERMINATED;

    kuts_printk("[SCH] Terminated process with pid %u\n", current_slot_index);
}

void sch_schedule() {
    schedule_trigger_ticks++;
    if (false == sch_is_running || schedule_trigger_ticks <= SCHEDULE_TRIGGER_TICK_PASS) {
        return;
    } else {
        schedule_trigger_ticks = 0;
    }

    handle_sleeping_processes();

    bool is_loaded_process_found = false;

    uint32_t next_slot_index = current_slot_index;
    uint32_t search_start_pid = current_slot_index;

    do {
        next_slot_index = (next_slot_index + 1) % SCH_MAX_LOADED_PROCESS;
        if (search_start_pid == next_slot_index) {
            if (is_idle && memmory_slots[next_slot_index].pcb.state == SCH_PSTATE_ACTIVE) {
                is_loaded_process_found = true;
            }
            break;
        }

        if (memmory_slots[next_slot_index].is_used == true) {
            if (memmory_slots[next_slot_index].pcb.state == SCH_PSTATE_ACTIVE ||
                memmory_slots[next_slot_index].pcb.state == SCH_PSTATE_NEW) {
                is_loaded_process_found = true;
                break;
            }
        }
    } while (is_loaded_process_found == false);

    if (is_loaded_process_found == true ||
        (loaded_process_num == 1 && memmory_slots[next_slot_index].pcb.state == SCH_PSTATE_NEW)) {
        is_idle = false;
        //kuts_printk("sch: switch to %u\n", next_slot_index);
        current_slot_index = next_slot_index;
        sch_next_pcb = &(memmory_slots[next_slot_index].pcb);

        if (sch_next_pcb->state == SCH_PSTATE_NEW) {
            sch_next_pcb->state = SCH_PSTATE_ACTIVE;
            sch_switch_new_process();
        } else {
            sch_switch_process();
        }
    } else if (memmory_slots[next_slot_index].pcb.state != SCH_PSTATE_ACTIVE &&
               is_idle == false) {
        is_idle = true;
        //kuts_printk("sch idle: %c %u\n", is_loaded_process_found, next_slot_index);
        sch_next_pcb = &idle_pcb;
        sch_switch_process();
    } else {
        //kuts_printk("sch keep running: %u %u\n", next_slot_index, (uint32_t)is_idle);
    }

    return;
}