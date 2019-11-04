#include <kernel/isr.h>
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

void sleep(size_t t) {
    for (size_t i = 0; i < t; i++) {
        printf("");
    }
}

void run_welcome_screen() {
    tty_set_char_color(VGA_COLOR_LIGHT_BLUE);
    printf("\n\n\n\n\n");
    printf(
        "\
              | |  | |    | |                           | |       \n\
              | |  | | ___| | ___ ___  _ __ ___   ___   | |_ ___  \n\
              | |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\  | __/ _ \\ \n\
              \\  /\\  /  __/ | (_| (_) | | | | | |  __/  | || (_) |\n\
               \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|   \\__\\___/ \n\
                                                                  \n\
                                                                  \n\
                          _                 _                     \n\
                         (_)               | |                    \n\
                      ___ _ _ __ ___  _ __ | | ___  ___           \n\
                     / __| | '_ ` _ \\| '_ \\| |/ _ \\/ __|          \n\
                     \\__ \\ | | | | | | |_) | | (_) \\__ \\          \n\
                     |___/_|_| |_| |_| .__/|_|\\___/|___/          \n\
                                     | |                          \n\
                                     |_|                          \n");
    tty_set_char_color(VGA_COLOR_WHITE);
    kuts_sleep(2000);
    tty_reset();
}

void kernel_main(void) {
    pg_init_paging();
    tty_init();
    ps2k_init();
    pitt_init();
    isr_idt_init();

    pitt_enable();

    run_welcome_screen();

    ps2k_start_buffering();

    char c;
    printf("> ");
    while (1) {
        if ((c = ps2k_get_char()) != EOF) {
            printf("%c", c);
        }
    }

    ps2k_stop_buffering();

    while (1)
        ;
}
