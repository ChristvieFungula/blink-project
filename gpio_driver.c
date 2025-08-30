#include "gpio_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>

typedef struct{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    uint8_t bit;
} gpio_sel_t;

static gpio_sel_t led = (gpio_sel_t){0};
static volatile uint16_t compare_value;

static int map_arduino_pin(uint8_t d, gpio_sel_t *out){
    switch (d){
        case 0:  out->ddr = &DDRD; out->port = &PORTD; out->bit = PD0; break;
        case 1:  out->ddr = &DDRD; out->port = &PORTD; out->bit = PD1; break;
        case 2:  out->ddr = &DDRD; out->port = &PORTD; out->bit = PD2; break;
        case 3:  out->ddr = &DDRD; out->port = &PORTD; out->bit = PD3; break;
        case 4:  out->ddr = &DDRD; out->port = &PORTD; out->bit = PD4; break;
        case 5:  out->ddr = &DDRD; out->port = &PORTD; out->bit = PD5; break;
        case 6:  out->ddr = &DDRD; out->port = &PORTD; out->bit = PD6; break;
        case 7:  out->ddr = &DDRD; out->port = &PORTD; out->bit = PD7; break;
        // D8..D13 -> PORTB0..PORTB5
        case 8:  out->ddr = &DDRB; out->port = &PORTB; out->bit = PB0; break;
        case 9:  out->ddr = &DDRB; out->port = &PORTB; out->bit = PB1; break;
        case 10: out->ddr = &DDRB; out->port = &PORTB; out->bit = PB2; break;
        case 11: out->ddr = &DDRB; out->port = &PORTB; out->bit = PB3; break;
        case 12: out->ddr = &DDRB; out->port = &PORTB; out->bit = PB4; break;
        case 13: out->ddr = &DDRB; out->port = &PORTB; out->bit = PB5; break;
        default: return 0;
    }
    return 1;
}

void gpio_init(uint8_t arduino_pin, delay_t delay){

    if(!map_arduino_pin(arduino_pin, &led)){
        map_arduino_pin(13, &led);
    }

    *(led.ddr) |= (1 << led.bit);
    *(led.port) &= ~(1 << led.bit);

    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);

    switch (delay){
    case DELAY_20MS: compare_value = 312; break;
    case DELAY_100MS: compare_value = 1563; break;
    case DELAY_500MS: compare_value = 7813; break;
    case DELAY_1S: compare_value = 15625; break;
    case DELAY_2S: compare_value = 31250; break;
    default: compare_value = 1563;
    }

    OCR1A = compare_value;
    TIMSK1 &= ~(1 << OCIE1A);
}
ISR(TIMER1_COMPA_vect){
   *(led.port) ^= (1 << led.bit);
}

void start_blinking(void){
    TIMSK1 |= (1 << OCIE1A);
    sei();    
}
void stop_blinking(void){
    TIMSK1 &= ~(1 << OCIE1A);
    *(led.port) &= ~(1 << led.bit);
}