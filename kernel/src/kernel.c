#include <kernel/isr.h>
#include <kernel/kernel_init.h>
#include <kernel/kernel_utils.h>
#include <kernel/paging.h>
#include <kernel/pit/pit_timer.h>
#include <kernel/ps2/keyboard.h>
#include <kernel/tty.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__linux__)
#error "You should use a ix86-elf cross-compiler"
#endif

#if !defined(__i386__)
#error "This program needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main(void) {
    pg_init_paging();
    tty_init();
    ps2k_init();
    pitt_init();
    isr_idt_init();

    pitt_enable();

    kinit_display_welcome_screen();

    ps2k_start_buffering();

    char c;
    printf("> ");
    while (1) {
        if ((c = ps2k_get_char()) != EOF) {
            if (c == '\n') {
                printf("\n\n> ");
            } else if (c == PS2K_UP_MAP_ASCII) {
                tty_scroll_up(1);
            } else if (c == PS2K_DOWN_MAP_ASCII) {
                tty_scroll_down(1);
            } else {
                printf("%c", c);
            }
        }
    }

    ps2k_stop_buffering();

    while (1)
        ;
}
