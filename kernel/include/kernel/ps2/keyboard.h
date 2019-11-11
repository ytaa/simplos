#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#define PS2K_IN_BUFFER_SIZE 256

//kernel_return_t values
#define PS2K_STATUS_BUFFER_FULL -2
#define PS2K_STATUS_BUFFER_EMPTY -3

#include <kernel/kernel_utils.h>
#include <simplstd.h>
#include <stdbool.h>
#include <stdint.h>
#include "US/keymap.h"

typedef struct {
    int pressed;
    unsigned char ascii;
    unsigned char s_ascii;
} ps2k_key;

typedef struct {
    uint8_t keycode;
    unsigned char character;
} ps2k_buffered_key;

void ps2k_init();
void ps2k_handle_irq1(uint8_t keycode);
int ps2k_is_pressed(uint8_t keycode);
void ps2k_start_buffering();
void ps2k_stop_buffering();
kernel_return_t ps2k_in_buffer_add(ps2k_buffered_key key);
kernel_return_t ps2k_in_buffer_get(ps2k_buffered_key *key);

#endif
