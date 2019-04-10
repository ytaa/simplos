#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#define EOF (-1)
#define IN_BUFFER_SIZE 256

#include <stdint.h>
#include "US/keymap.h"

typedef struct {
  int pressed;
  unsigned char ascii;
  unsigned char s_ascii;
} ps2k_key;

void ps2k_init();
void ps2k_handle_irq1(uint8_t keycode);
int ps2k_is_pressed(uint8_t keycode);
void ps2k_start_buffering();
void ps2k_stop_buffering();
char ps2k_get_char();
void ps2k_in_buffer_add(char c);
char ps2k_in_buffer_get();
#endif
