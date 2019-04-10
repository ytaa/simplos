#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

static size_t tty_vga_row;
static size_t tty_vga_column;
static uint8_t tty_vga_color;
static uint16_t* tty_vga_buffer;

static uint16_t tty_buffer[TTY_WIDTH*TTY_HEIGHT];
static vga_color tty_char_color;
static vga_color tty_background_color;
static size_t tty_row;
static size_t tty_column;
static size_t tty_row_offset;
static size_t tty_column_offset;

void tty_init(void) {
	tty_vga_row = 0;
	tty_vga_column = 0;
	tty_column = 0;
	tty_row = 0;
	tty_column_offset = 0;
	tty_row_offset = 0;
	tty_char_color = VGA_COLOR_LIGHT_GREY;
	tty_background_color = VGA_COLOR_BLACK;
	tty_vga_color = vga_make_color(tty_char_color, tty_background_color);
	tty_vga_buffer = VGA_TEXT_BUFFER;
	for (size_t index = 0; index < VGA_WIDTH*VGA_HEIGHT; index++) {
		tty_vga_buffer[index] = vga_make_entry(' ', tty_vga_color);
	}
	for (size_t index = 0; index < TTY_WIDTH*TTY_HEIGHT; index++) {
		tty_buffer[index] = vga_make_entry(' ', tty_vga_color);
	}
}

void tty_set_char_color(vga_color color) {
	tty_char_color = color;
	tty_vga_color = vga_make_color(tty_char_color, tty_background_color);
}

void tty_set_background_color(vga_color color) {
	tty_background_color = color;
	tty_vga_color = vga_make_color(tty_char_color, tty_background_color);
}

size_t tty_get_scroll_vertical(){
	return tty_row_offset;
}
size_t tty_get_scroll_horizontal(){
	return tty_column_offset;
}

void tty_scroll_down(size_t amount){
	if(tty_row_offset + amount >= TTY_HEIGHT - VGA_HEIGHT){
		tty_row_offset = TTY_HEIGHT - VGA_HEIGHT - 1;
	}
	else{
		tty_row_offset += amount;
	}
	tty_rewrite_vga_buffer();
}

void tty_scroll_up(size_t amount){
	if((int)(tty_row_offset - amount) < 0){
		tty_row_offset = 0;
	}
	else{
		tty_row_offset -= amount;
	}
	tty_rewrite_vga_buffer();
}

void tty_scroll_left(size_t amount){
	if((int)(tty_column_offset - amount) < 0){
		tty_column_offset = 0;
	}
	else{
		tty_column_offset -= amount;
	}
	tty_rewrite_vga_buffer();
}

void tty_scroll_right(size_t amount){
	if(tty_column_offset + amount >= TTY_WIDTH - VGA_WIDTH){
		tty_column_offset = TTY_WIDTH - VGA_WIDTH - 1;
	}
	else{
		tty_column_offset += amount;
	}
	tty_rewrite_vga_buffer();
}

void tty_set_vga_color(vga_color char_color, vga_color background_color) {
	tty_char_color = char_color;
	tty_background_color = background_color;
	tty_vga_color = vga_make_color(tty_char_color, tty_background_color);
}

void tty_rewrite_vga_buffer(){
	for(size_t row = 0; row < VGA_HEIGHT; row++){
		for(size_t col = 0; col < VGA_WIDTH; col++){
			tty_vga_buffer[row*VGA_WIDTH + col] = tty_buffer[(row + tty_row_offset)*TTY_WIDTH + col + tty_column_offset];
		}
	}
}

void tty_put_entry(unsigned char c, size_t row, size_t column){
	const size_t index = row*TTY_WIDTH + column;
	tty_buffer[index] = vga_make_entry(c, tty_vga_color);
	const int vga_row = row - tty_row_offset;
	const int vga_column = column - tty_column_offset;
	if(vga_row >= 0 && vga_column >= 0 && vga_row < VGA_HEIGHT && vga_column < VGA_WIDTH){
		const size_t vga_index = vga_row*VGA_WIDTH + vga_column;
		tty_vga_buffer[vga_index] = tty_buffer[index];
	}
}

void tty_put_char(char c){
	unsigned char uc = c;
	if(uc == '\n'){
		tty_column = 0;
		tty_row++;
	}else{
		tty_put_entry(uc, tty_row, tty_column);
		if(++tty_column >= TTY_WIDTH){
			tty_column = 0;
			tty_row++;
		}
	}
	if(tty_row >= TTY_HEIGHT){
		tty_row = 0;
		tty_row_offset = 0;
		tty_rewrite_vga_buffer();
	}
	if((int)(tty_row - tty_row_offset) >= VGA_HEIGHT){
		tty_scroll_down(tty_row - tty_row_offset - VGA_HEIGHT + 1);
	}
}

void tty_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++){
		tty_put_char(data[i]);
	}
}

void tty_write_string(const char* str) {
	tty_write(str, strlen(str));
}
