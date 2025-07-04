#include "gpio_driver.h"

int main(void) {
    gpio_init(DELAY_500MS);
    start_blinking();

    while (1) {
    }
}
