#include <kernel/pit/pit_timer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/io.h>

static bool is_initalized = false;
static bool is_enabled = false;

static struct SystemTime current_system_time = {0, 0, 0, 0};
static uint64_t current_milisecond = 0;

void pitt_init(void) {
    if (is_initalized == false) {
        //program the PIT channel 0 to requested frequency

        //calculate reload value: reload = 1193182 / freq
        uint16_t reload = PITT_RELOAD_DENOMINATOR / PITT_CH0_FREQUENCY_HZ;

        //select chanel
        outb(PITT_MODE_PORT, PITT_CH0_PROGRAM_MODE_VALUE);

        //send low byte
        outb(PITT_CH0_PORT, (uint8_t)reload);

        //send high byte
        outb(PITT_CH0_PORT, (uint8_t)(reload >> 8));

        is_initalized = true;
    }
}

void pitt_enable(void) {
    if (is_initalized && is_enabled == false) {
        bzero(&current_system_time, sizeof(current_system_time));
        current_milisecond = 0;
        is_enabled = true;
    }
}

void pitt_disable(void) {
    is_enabled = false;
}

void pitt_handle_irq(void) {
    if (is_enabled) {
        current_milisecond += PITT_MS_PER_CH0_TICK;
        current_system_time.milliseconds += PITT_MS_PER_CH0_TICK;
        if (current_system_time.milliseconds >= PITT_MAX_MS) {
            current_system_time.milliseconds = 0;
            current_system_time.seconds++;
            if (current_system_time.seconds >= PITT_MAX_SEC) {
                current_system_time.seconds = 0;
                current_system_time.minutes++;
                if (current_system_time.minutes >= PITT_MAX_MIN) {
                    current_system_time.minutes = 0;
                    current_system_time.hours++;
                    if (current_system_time.hours >= PITT_MAX_H) {
                        current_system_time.hours = 0;
                    }
                }
            }
        }
    }
}

void pitt_get_system_time(struct SystemTime *system_time) {
    *system_time = current_system_time;
}

void pitt_get_current_milisecond(uint64_t *milisecond) {
    *milisecond = current_milisecond;
}