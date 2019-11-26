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

int32_t syscall(uint32_t opcode, syscall_params *params);

int32_t exec(uint32_t program_index);

int32_t sleep(uint32_t time_ms);

int32_t wait();

#ifdef __cplusplus
}
#endif

#endif
