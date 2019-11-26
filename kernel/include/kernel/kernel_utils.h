#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include <stdint.h>

#define KUTS_MAX_MS 0xFFFFFFFFFFFFFFFF

#define KERNEL_STATUS_SUCCESS 0
#define KERNEL_STATUS_FAILURE -1

typedef int16_t kernel_return_t;

void kuts_sleep(uint64_t sleep_ms);

uint64_t kuts_calc_ms_diff(uint64_t start_ms, uint64_t end_ms);

int kuts_printk(const char* format, ...);

#endif