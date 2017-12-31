#include "delay.h"

extern void msx_wait_jiffies(uint16_t j);

static inline uint8_t vdp_is_50hz() {
    uint8_t *data = (uint8_t *) 0xFFE8;
    return ((*data) & 0x02);
}

void delay(uint16_t ms) {
    uint16_t vblank_freq = vdp_is_50hz() ? 20 : 16;

    uint16_t jiffies = ms / ((uint16_t) vblank_freq);

    if (jiffies > 0) {
        msx_wait_jiffies((uint16_t) jiffies);
    }
}