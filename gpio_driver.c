#include "gpio_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN PD2

static volatile uint16_t compare_value;

void gpio_init(delay_t delay){
    LED_DDR |= (1 << LED_PIN);
    LED_PORT &= ~(1 << LED_PIN);


    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12);

    switch (delay)
    {
    case DELAY_20MS: compare_value = 1250; break;
    case DELAY_100MS: compare_value = 6250; break;
    case DELAY_500MS: compare_value = 31250; break;
    case DELAY_1S: compare_value = 62500; break;
    case DELAY_2S: compare_value = 125000; break;
    default: compare_value = 6250;
    }

    OCR1A = compare_value;
    TIMSK1 |= (1 << OCIE1A);
    sei();
}
ISR(TIMER1_COMPA_vect){
    LED_PORT ^= (1 << LED_PIN);
}

void start_blinking(void){

}