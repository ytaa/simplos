#include <boot/tss.h>
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

uint32_t *sbuf1;
uint32_t *sbuf2;

uint32_t sbuf11[10];
uint32_t sbuf22[10];

void kernel_main(void) {
    //tss_init();
    pg_init_paging();

    tty_init();
    ps2k_init();
    pitt_init();
    isr_idt_init();
    sch_init();

    pitt_enable();

    kinit_display_welcome_screen();

    ps2k_start_buffering();

    sbuf1 = sbuf11;
    sbuf2 = sbuf22;

    sch_request_program_index = 0;
    sch_request_exec();

    sch_run();

    ps2k_stop_buffering();

    while (1)
        ;
}
