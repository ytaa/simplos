#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/isr.h>
#include <kernel/ps2/keyboard.h>

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
  ps2k_init();
  isr_idt_init();
  printf("Hello kernel\n");
  ps2k_start_buffering();
  char c;
  while(1){
    if((c = ps2k_get_char()) != EOF){
      printf("%c", c);
    }
  }
  ps2k_stop_buffering();
  while(1);
}
