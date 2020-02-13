#pragma once

#include "stm32g4xx_hal.h"

class Pin {
public:
    constexpr Pin(GPIO_TypeDef* port, int pin) : _pin(pin), _port(port) {}
    constexpr uint16_t pin() const { return (1 << _pin); }
    constexpr GPIO_TypeDef* port() const { return const_cast<GPIO_TypeDef*>(_port); }
    Pin set_alternate(uint32_t alt_function) { 
        if (_pin < 8) {
            LL_GPIO_SetAFPin_0_7(_port, pin(), alt_function);
        } else {
            LL_GPIO_SetAFPin_8_15(_port, pin(), alt_function);
        }
        return *this;
    }

private:
    const int _pin;
    const GPIO_TypeDef* _port;
};


enum PinMode {
    Input = GPIO_MODE_INPUT,
    Output = GPIO_MODE_OUTPUT_PP,
    Analog = GPIO_MODE_ANALOG,
};

template<PinMode TMODE>
class Gpio {
public:
    explicit Gpio(const Pin& pin, bool initial=false)
    : _pin{pin} {
        GPIO_InitTypeDef init = {
            .Pin = pin.pin(),
            .Mode = TMODE,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_LOW,
        };
        HAL_GPIO_Init(pin.port(), &init);

        if constexpr (TMODE == Output) {
            write(initial);
        }
    }

    void write(bool state) {
        auto pinstate = state ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(_pin.port(), _pin.pin(), pinstate);
    }

    bool read() const {
        return HAL_GPIO_ReadPin(_pin.port(), _pin.pin()) ? true : false;
    }

private:
    const Pin& _pin;
};