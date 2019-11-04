#include <string.h>

void bzero(void *buffer, size_t num_bytes) {
    memset(buffer, 0, num_bytes);
}