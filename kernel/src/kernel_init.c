#include <kernel/kernel_init.h>

#include <kernel/kernel_utils.h>
#include <kernel/ps2/keyboard.h>
#include <kernel/tty.h>
#include <stdio.h>

void kinit_display_welcome_screen(void) {
    tty_set_char_color(KINIT_WELCOME_SCREEN_COLOR);
    kuts_printk(KINIT_WELCOME_MSG);
    kuts_sleep(KINIT_WELCOME_SCREEN_DURATION_MS);
    tty_reset();
}