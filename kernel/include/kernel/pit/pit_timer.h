#ifndef PIT_TIMER_H
#define PIT_TIMER_H

#include <stdint.h>

#define PITT_CH0_FREQUENCY_HZ ((uint32_t)100)  //min:18, max: 1000
#define PITT_CH0_PROGRAM_MODE_VALUE 0x36
#define PITT_CH0_PORT 0x40

#define PITT_MODE_PORT 0x43
#define PITT_RELOAD_BASE_FREQUENCY ((uint32_t)1193182)

#define PITT_MAX_MS 1000u
#define PITT_MAX_SEC 60u
#define PITT_MAX_MIN 60u
#define PITT_MAX_H 25u

#define PITT_MS_PER_CH0_TICK PITT_MAX_MS / PITT_CH0_FREQUENCY_HZ

struct SystemTime {
    uint16_t milliseconds;
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
};

void pitt_init(void);

void pitt_enable(void);

void pitt_disable(void);

void pitt_handle_irq(void);

void pitt_get_system_time(struct SystemTime *system_time);

void pitt_get_current_milisecond(uint64_t *milisecond);

#endif