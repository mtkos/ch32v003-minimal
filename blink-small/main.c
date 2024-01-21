#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"

#define PIN 3

void prog(){

    REG(RCC + APB2PCENR) = IOPCEN;

    REG(GPIOC + CFGLR) &= Clear(PIN);
    REG(GPIOC + CFGLR) |= Output_PP_2MHz(PIN);

    while(1){
        for(volatile uint32_t i = 100000; i > 0; i--);
        REG(GPIOC + OUTDR) ^= Pin(PIN);
    }
}
