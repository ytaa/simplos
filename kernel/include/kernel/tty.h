#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <vga.h>

#define TTY_WIDTH VGA_WIDTH
#define TTY_HEIGHT 10000
#define TTY_TAB_SIZE 5
#define TTY_DEFAULT_CHAR_COLOR VGA_COLOR_LIGHT_GREY
#define TTY_DEFAULT_BACKGROUND_COLOR VGA_COLOR_BLACK

void tty_init(void);
void tty_reset(void);
void tty_set_char_color(vga_color color);
void tty_set_background_color(vga_color color);
void tty_set_vga_color(vga_color char_color, vga_color background_color);
void tty_enable_vga_cursor(uint8_t start, uint8_t end);
void tty_disable_vga_cursor();
void tty_set_vga_cursor_pos(int row, int col);
size_t tty_get_scroll_vertical();
size_t tty_get_scroll_horizontal();
void tty_scroll_up(size_t amount);
void tty_scroll_down(size_t amount);
void tty_scroll_left(size_t amount);
void tty_scroll_right(size_t amount);
void tty_rewrite_vga_buffer(void);
void tty_put_entry(unsigned char c, size_t row, size_t col);
void tty_put_char(char c);
void tty_write(const char* data, size_t size);
void tty_write_string(const char* str);

#endif
