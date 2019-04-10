#ifndef _STDIO_H
#define _STDIO_H 1

#include <kernel/ps2/keyboard.h>

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char*, ...);
int putchar(int);
int puts(const char*);
int getchar();

#ifdef __cplusplus
}
#endif

#endif
