#include "gpio_driver.h"

int main(void) {
    gpio_init(15, DELAY_500MS);
    start_blinking();

    while (1) {
    }
}
