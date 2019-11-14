#include <elf/elf32_loader.h>
#include <elf/elf_test.h>
#include <kernel/isr.h>
#include <kernel/kernel_init.h>
#include <kernel/kernel_utils.h>
#include <kernel/paging.h>
#include <kernel/pit/pit_timer.h>
#include <kernel/ps2/keyboard.h>
#include <kernel/scheduler.h>
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

void kernel_loop(void) {
    ps2k_buffered_key key;
    printf("> ");
    while (1) {
        if (ps2k_in_buffer_get(&key) >= 0) {
            if (key.character != 0) {
                if (key.character == '\n') {
                    printf("\n\n> ");
                } else {
                    printf("%c", key.character);
                }
            } else {
                if (key.keycode == PS2K_KEYCODE_UP) {
                    tty_scroll_up(1);
                } else if (key.keycode == PS2K_KEYCODE_DOWN) {
                    tty_scroll_down(1);
                }
            }
        }
    }
}

void kernel_main(void) {
    pg_init_paging();
    tty_init();
    ps2k_init();
    pitt_init();
    isr_idt_init();
    sch_init();

    pitt_enable();

    kinit_display_welcome_screen();

    ps2k_start_buffering();

    elf32_info elf_info;
    elf32_load_exec(test_elf_buffer, 134512640, &elf_info);
    printf("Actual base vaddr: %d\n", elf_info.base_vaddr);
    printf("kernel_loop addr: %u\n", (uint32_t)kernel_loop);
    printf("jumping to %u...\n", elf_info.entry_vaddr);

    asm volatile(
        "jmp *%0"
        :
        : "rm"(elf_info.entry_vaddr));

    kernel_loop();

    ps2k_stop_buffering();

    while (1)
        ;
}
