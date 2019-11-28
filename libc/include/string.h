#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//int memcmp(const void*, const void*, size_t);

void* memcpy(void* __restrict dst, const void* __restrict src, size_t nbytes);

void* memset(void* buffer, int byte_value, size_t num_bytes);

void bzero(void* buffer, size_t num_bytes);

size_t strlen(const char* str);

int strcmp(const char* str1, const char* str2);

#ifdef __cplusplus
}
#endif

#endif
