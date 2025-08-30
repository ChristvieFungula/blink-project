#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

typedef enum {
    DELAY_20MS,
    DELAY_100MS,
    DELAY_500MS,
    DELAY_1S,
    DELAY_2S
} delay_t;

void gpio_init(uint8_t arduino_pin, delay_t delay);
void start_blinking(void);
void stop_blinking(void);

#endif
