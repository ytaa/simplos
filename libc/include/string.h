#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//int memcmp(const void*, const void*, size_t);

//void* memcpy(void* __restrict, const void* __restrict, size_t);

//void* memmove(void*, const void*, size_t);

void* memset(void* buffer, int byte_value, size_t num_bytes);

void bzero(void* buffer, size_t num_bytes);

size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif

#endif
