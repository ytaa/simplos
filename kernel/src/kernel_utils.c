#include <kernel/kernel_utils.h>
#include <kernel/pit/pit_timer.h>

void kuts_sleep(uint64_t sleep_ms) {
    uint64_t start_ms = 0;
    uint64_t current_ms = 0;

    pitt_get_current_milisecond(&start_ms);

    do {
        pitt_get_current_milisecond(&current_ms);
    } while (sleep_ms > kuts_calc_ms_diff(start_ms, current_ms));
}

uint64_t kuts_calc_ms_diff(uint64_t start_ms, uint64_t end_ms) {
    uint64_t delta_ms = 0;

    if (start_ms <= end_ms) {
        delta_ms = end_ms - start_ms;
    } else {
        //start_ms > end_ms => buffer overflow
        delta_ms += KUTS_MAX_MS - start_ms;
        delta_ms += end_ms;
    }

    return delta_ms;
}