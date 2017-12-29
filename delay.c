#include <math.h>
#include "delay.h"

extern void msx_wait_jiffies(uint16_t j);

static inline float round(float d)
{
    if (d > 0.0)
        return floorf(d + 0.5);
    else if (d < 0.0)
        return ceilf(d - 0.5);
    else
        return d;
}

static inline uint8_t vdp_is_50hz() {
	uint8_t *data = (uint8_t *) 0xFFE8;
	return ((*data) & 0x02);
}

void delay(uint16_t ms) {
	float vblank_freq = vdp_is_50hz() ? 20.0 : 16.66;

	uint16_t jiffies = (uint16_t) round(((float) ms) / vblank_freq);

    if(jiffies > 0) {
        msx_wait_jiffies((uint16_t) jiffies);
    }
}