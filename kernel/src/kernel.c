#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/tty.h>

#if defined(__linux__)
#error "You should use a ix86-elf cross-compiler"
#endif

#if !defined(__i386__)
#error "This program needs to be compiled with a ix86-elf compiler"
#endif

void sleep(size_t t){
  for(size_t i = 0; i<t; i++){
    printf("");
  }
}

void kernel_main(void)
{
  tty_init();
  char *str = "hello string";
  int d = -1001033408;
  char c = 'x';
  size_t sleep_time = 2000000;
  for(size_t i = 0; i<VGA_HEIGHT/2; i++){
    printf("\n");
  }
  for(size_t i=0; i<VGA_WIDTH/2-strlen(" Hello world ")/2; i++){
    printf(" ");
  }
  tty_set_char_color(VGA_COLOR_LIGHT_MAGENTA);
  tty_set_background_color(VGA_COLOR_DARK_GREY);
  printf(" Hello world \n");
  for(int i = 0; i<VGA_HEIGHT/2; i++){
    printf("\n");
  }
  tty_set_char_color(VGA_COLOR_LIGHT_GREY);
  tty_set_background_color(VGA_COLOR_BLACK);
  sleep(sleep_time*10);
  printf("This is a string: '%s', this is a char: '%c' and this is a int: '%d'\n", str, c, d);
  //while(1);
  sleep(sleep_time);
  for(size_t i = 0; i<VGA_HEIGHT+10; i++){
    printf("%d:\n", i);
    sleep(sleep_time);
  }
  size_t max_scroll = tty_get_scroll_vertical();
  while(1){
    while(tty_get_scroll_vertical() > 0){
        tty_scroll_up(1);
        sleep(sleep_time);
    }
    while(tty_get_scroll_vertical() < max_scroll){
      tty_scroll_down(1);
      sleep(sleep_time);
    }
  }
}
