#include <kernel/ps2/keyboard.h>
#include <kernel/scheduler.h>
#include <kernel/stdin.h>
#include <kernel/tty.h>
#include <simplstd.h>
#include <stdint.h>

#define BACKSPACE_ASCII 8

static char stdin_buffer[STDIN_BUFFER_SIZE];
static uint32_t buffered_characters;
static ps2k_buffered_key key;

void stdin_update() {
    if (ps2k_in_buffer_get(&key) >= 0) {
        if (key.character != 0) {
            if (key.character == '\n') {
                //notify processes waitning for user input
                stdin_flush();
            } else if (key.character == BACKSPACE_ASCII) {
                if (buffered_characters > 0) {
                    tty_backspace();
                    buffered_characters--;
                }
            } else {
                if (buffered_characters < STDIN_BUFFER_SIZE - 1) {
                    stdin_buffer[buffered_characters] = key.character;
                    buffered_characters++;
                    tty_put_char(key.character);
                }
            }
            stdin_buffer[buffered_characters] = '\0';
        } else {
            if (key.keycode == PS2K_KEYCODE_UP) {
                tty_scroll_up(1);
            } else if (key.keycode == PS2K_KEYCODE_DOWN) {
                tty_scroll_down(1);
            }
        }
    }
}

void stdin_flush() {
    //notify processes waitning for user input
    tty_put_char('\n');
    sch_flush_input((uint8_t *)stdin_buffer, buffered_characters);
    stdin_clear();
}

void stdin_clear() {
    buffered_characters = 0;
    stdin_buffer[buffered_characters] = '\0';
}