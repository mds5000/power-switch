#include "gpio.h"

#include "stm32g4xx.h"


auto led = Gpio<Output>(Pin(GPIOA, 4));

int main() {

    while(true) {
        led.write(true);
    }

    return 0;
}