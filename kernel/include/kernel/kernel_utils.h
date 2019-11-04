#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include <stdint.h>

#define KUTS_MAX_MS 0xFFFFFFFFFFFFFFFF

void kuts_sleep(uint64_t sleep_ms);

uint64_t kuts_calc_ms_diff(uint64_t start_ms, uint64_t end_ms);

#endif