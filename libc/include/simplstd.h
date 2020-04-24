#ifndef _SIMPLSTD_H
#define _SIMPLSTD_H

#include <stdint.h>

//syscall opcodes
#define SYSCALL_SYS_EXIT 1
#define SYSCALL_SYS_WRITE 2
#define SYSCALL_SYS_EXEC 3
#define SYSCALL_SYS_READ 4
#define SYSCALL_SYS_MSSLEEP 5
#define SYSCALL_SYS_WAIT 6

//syscall return values
#define SYSCALL_STATUS_SUCCESS 0
#define SYSCALL_STATUS_INVALID_PARAM -2
#define SYSCALL_STATUS_INVALID_OPCODE -3
#define SYSCALL_INVALID_FD -4

//stdin
#define STDIN_BUFFER_SIZE 256

//standard file descriptors
#define STDOUT 1
#define STDIN 2

#define EOF -1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint32_t param4;
    uint32_t param5;
} syscall_params;

typedef enum {
    prog_id_shell = 0,
    prog_id_test2,
    prog_id_input,
    prog_id_test,
    prog_id_testcase1,
    prog_id_testcase2,
    prog_id_testcase3,
    prog_id_testcase3_1,
    prog_id_testcase4,
    prog_id_testcase4_1,
    prog_id_testcase4_2,
    prog_id_testcase5,
    prog_id_testcase5_1,
    prog_id_testcase5_2,
    prog_id_testcase5_3,
    prog_id_testcase6,
    prog_id_testcase7,
    prog_id_testcase7_1,
    prog_id_testcase8,
    prog_id_testcase9,

    prog_id_count
} program_id;

int32_t syscall(uint32_t opcode, syscall_params *params);

int32_t exec(uint32_t program_index);

int32_t sleep(uint32_t time_ms);

int32_t wait();

int32_t exit();

#ifdef __cplusplus
}
#endif

#endif
