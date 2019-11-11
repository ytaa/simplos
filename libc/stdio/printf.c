#include <limits.h>
#include <simplstd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int printf(const char* format, ...) {
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
                    if (putchar(str[j]) == EOF) {
                        return -1;
                    }
                }
                written += len;
            } else if (format_char == 'c') {
                char c = (char)va_arg(parameters, int);
                if (putchar(c) == EOF) {
                    return -1;
                }
                written++;
            } else if (format_char == 'd') {
                int d = va_arg(parameters, int);
                if (d == 0) {
                    if (putchar('0') == EOF) {
                        return -1;
                    }
                    written++;
                } else {
                    if (d < 0) {
                        d = -d;
                        if (putchar('-') == EOF) {
                            return -1;
                        }
                    }
                    bool begining_found = false;
                    for (long long int i = 1000000000; i > 0; i /= 10) {
                        int number = d / i;
                        if (number != 0) {
                            begining_found = true;
                            if (putchar(48 + number) == EOF) {
                                return -1;
                            }
                            written++;
                        } else {
                            if (begining_found) {
                                if (putchar('0') == EOF) {
                                    return -1;
                                }
                                written++;
                            }
                        }
                        d -= number * i;
                    }
                }
            } else {
                i--;
            }
        } else {
            if (putchar(cr_char) == EOF) {
                return -1;
            }
            written++;
        }
    }
    va_end(parameters);
    return written;
}
