#include <kernel/kernel_utils.h>
#include <kernel/pit/pit_timer.h>
#include <kernel/tty.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

int kuts_printk(const char* format, ...) {
    va_list parameters;
    va_start(parameters, format);
    int written = 0;
    int formatLength = strlen(format);
    for (int i = 0; i < formatLength; i++) {
        char cr_char = format[i];
        if (cr_char == '%') {
            char format_char = format[i + 1];
            i++;
            if (format_char == 's') {
                const char* str = va_arg(parameters, const char*);
                size_t len = strlen(str);
                for (size_t j = 0; j < len; j++) {
                    tty_put_char(str[j]);
                }
                written += len;
            } else if (format_char == 'c') {
                char c = (char)va_arg(parameters, int);
                tty_put_char(c);
                written++;
            } else if (format_char == 'd') {
                int d = va_arg(parameters, int);
                if (d == 0) {
                    tty_put_char('0');
                    written++;
                } else {
                    if (d < 0) {
                        d = -d;
                        tty_put_char('-');
                    }
                    bool begining_found = false;
                    for (long long int i = 1000000000; i > 0; i /= 10) {
                        int number = d / i;
                        if (number != 0) {
                            begining_found = true;
                            tty_put_char(48 + number);
                            written++;
                        } else {
                            if (begining_found) {
                                tty_put_char('0');
                                written++;
                            }
                        }
                        d -= number * i;
                    }
                }
            } else if (format_char == 'u') {
                uint32_t u = va_arg(parameters, uint32_t);
                if (u == 0) {
                    tty_put_char('0');
                    written++;
                } else {
                    bool begining_found = false;
                    for (uint64_t i = 10000000000; i > 0; i /= 10) {
                        uint32_t number = u / i;
                        if (number != 0) {
                            begining_found = true;
                            tty_put_char(48 + number);
                            written++;
                        } else {
                            if (begining_found) {
                                tty_put_char('0');
                                written++;
                            }
                        }
                        u -= number * i;
                    }
                }
            } else {
                i--;
            }
        } else {
            tty_put_char(cr_char);
            written++;
        }
    }
    va_end(parameters);
    return written;
}
