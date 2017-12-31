#include "delay.h"

extern void msx_wait_jiffies(uint16_t j);

static uint8_t *data = (uint8_t *) 0xFFE8;

void delay(uint16_t ms) {
    uint16_t vblank_freq, jiffies;

    if((*data) & 0x02) {
        vblank_freq = 20;
    } else {
        vblank_freq = 16;
    }

    jiffies = ms / vblank_freq;

    if (jiffies > 0) {
        msx_wait_jiffies(jiffies);
    }
}