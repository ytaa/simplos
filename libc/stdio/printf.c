#include <limits.h>
#include <simplstd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PUTCHAR_BUFFER_SIZE 256u

static char putchar_buffer[PUTCHAR_BUFFER_SIZE];
static uint32_t putchar_buffer_index = 0u;

static int flush_putchar_buffer() {
    int status = puts(putchar_buffer);

    putchar_buffer_index = 0;
    bzero(putchar_buffer, PUTCHAR_BUFFER_SIZE);

    return status;
}

static int buffered_putchar(int ic) {
    putchar_buffer[putchar_buffer_index++] = ic;

    if (putchar_buffer_index >= PUTCHAR_BUFFER_SIZE - 1u) {
        return flush_putchar_buffer();
    }

    return 0;
}

int printf(const char* format, ...) {
    bzero(putchar_buffer, PUTCHAR_BUFFER_SIZE);
    putchar_buffer_index = 0u;

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
                    if (buffered_putchar(str[j]) == EOF) {
                        return -1;
                    }
                }
                written += len;
            } else if (format_char == 'c') {
                char c = (char)va_arg(parameters, int);
                if (buffered_putchar(c) == EOF) {
                    return -1;
                }
                written++;
            } else if (format_char == 'd') {
                int d = va_arg(parameters, int);
                if (d == 0) {
                    if (buffered_putchar('0') == EOF) {
                        return -1;
                    }
                    written++;
                } else {
                    if (d < 0) {
                        d = -d;
                        if (buffered_putchar('-') == EOF) {
                            return -1;
                        }
                    }
                    bool begining_found = false;
                    for (long long int i = 1000000000; i > 0; i /= 10) {
                        int number = d / i;
                        if (number != 0) {
                            begining_found = true;
                            if (buffered_putchar(48 + number) == EOF) {
                                return -1;
                            }
                            written++;
                        } else {
                            if (begining_found) {
                                if (buffered_putchar('0') == EOF) {
                                    return -1;
                                }
                                written++;
                            }
                        }
                        d -= number * i;
                    }
                }
            } else if (format_char == 'u') {
                uint32_t u = va_arg(parameters, uint32_t);
                if (u == 0) {
                    if (buffered_putchar('0') == EOF) {
                        return -1;
                    }
                    written++;
                } else {
                    bool begining_found = false;
                    for (uint64_t i = 10000000000; i > 0; i /= 10) {
                        uint32_t number = u / i;
                        if (number != 0) {
                            begining_found = true;
                            if (buffered_putchar(48 + number) == EOF) {
                                return -1;
                            }
                            written++;
                        } else {
                            if (begining_found) {
                                if (buffered_putchar('0') == EOF) {
                                    return -1;
                                }
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
            if (buffered_putchar(cr_char) == EOF) {
                return -1;
            }
            written++;
        }
    }
    va_end(parameters);

    if (putchar_buffer_index != 0u) {
        if (flush_putchar_buffer() == EOF) {
            return -1;
        }
    }

    return written;
}
