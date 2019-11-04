#include <stdint.h>
#include <string.h>
void* memset(void* buffer, int c, size_t num_bytes) {
    uint8_t byte_value = (uint8_t)c;
    uint8_t* casted_buffer = (uint8_t*)buffer;
    for (uint64_t byte_index = 0; byte_index < num_bytes; byte_index++, casted_buffer++) {
        *casted_buffer = byte_value;
    }
    return buffer;
}