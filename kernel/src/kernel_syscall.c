#include <kernel/kernel_syscall.h>
#include <kernel/scheduler.h>
#include <kernel/tty.h>
#include <stdio.h>

extern void kernel_syscall_read_params();

volatile uint32_t kernel_syscall_opcode;
volatile syscall_params kernel_syscall_params;
volatile syscall_params* kernel_syscall_params_ptr;

static int32_t sys_exit() {
    sch_terminate_process();
    return KERNEL_STATUS_SUCCESS;
}

static int32_t sys_write(uint32_t fd, uint8_t* data, uint32_t size) {
    int32_t status = SYSCALL_STATUS_SUCCESS;

    if (fd == STDOUT) {
        tty_write((const char*)data, size);
    } else {
        status = SYSCALL_INVALID_FD;
    }

    return status;
}

static int32_t sys_read(uint32_t fd, uint8_t* data, uint32_t size, bool* is_blocked) {
    int32_t status = SYSCALL_STATUS_SUCCESS;
    //kuts_printk("ok %u\n", is_blocked);
    if (fd == STDIN) {
        (void)size;
        sch_input_block_current_process(data, is_blocked);
    } else {
        status = SYSCALL_INVALID_FD;
    }

    return status;
}

static int32_t sys_exec(uint32_t program_index) {
    sch_request_exec_syscall(program_index);
    return 0;
}

static int32_t sys_mssleep(uint32_t sleep_ms, bool* is_sleeping) {
    int32_t status = SYSCALL_STATUS_SUCCESS;

    sch_sleep_current_process(sleep_ms, is_sleeping);

    return status;
}

static int32_t sys_wait(bool* is_blocked) {
    int32_t status = SYSCALL_STATUS_SUCCESS;

    sch_wait_children_block_current_process(is_blocked);

    return status;
}

int32_t kernel_syscall(uint32_t opcode, volatile syscall_params* prs) {
    int32_t status = SYSCALL_STATUS_INVALID_PARAM;
    if (prs) {
        status = SYSCALL_STATUS_INVALID_OPCODE;
        switch (opcode) {
            case SYSCALL_SYS_EXIT:
                status = sys_exit();
                break;
            case SYSCALL_SYS_WRITE:
                status = sys_write(prs->param1, (uint8_t*)(prs->param2), prs->param3);
                break;
            case SYSCALL_SYS_READ:
                status = sys_read(prs->param1, (uint8_t*)(prs->param2), prs->param3, (bool*)prs->param4);
                break;
            case SYSCALL_SYS_EXEC:
                status = sys_exec(prs->param1);
                break;
            case SYSCALL_SYS_MSSLEEP:
                status = sys_mssleep(prs->param1, (bool*)prs->param2);
                break;
            case SYSCALL_SYS_WAIT:
                status = sys_wait((bool*)prs->param1);
                break;
            default:
                break;
        }
    }
    return status;
}

void kernel_syscall_irq_handler() {
    kernel_syscall_params_ptr = &kernel_syscall_params;
    kernel_syscall_read_params();

    asm volatile(
        "movl %0, %%eax"
        :
        : "r"(kernel_syscall(kernel_syscall_opcode, &kernel_syscall_params)));
}