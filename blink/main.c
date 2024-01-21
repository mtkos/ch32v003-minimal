#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"

#define PIN3 3
#define PIN5 5

uint32_t count = 1000;

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

uint32_t period(){

    count += 1000;
    return count;
}

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);

}

void prog(){

    flash_to_ram();

    REG(RCC + APB2PCENR) |= IOPCEN;

    REG(GPIOC + CFGLR) &= Clear(PIN3);
    REG(GPIOC + CFGLR) |= Output_PP_2MHz(PIN3);
    REG(GPIOC + CFGLR) &= Clear(PIN5);
    REG(GPIOC + CFGLR) |= Output_PP_2MHz(PIN5);

    REG(GPIOC + OUTDR) = Pin(PIN3);
    while(1){
        for(volatile uint32_t i = period(); i > 0; i--);
        REG(GPIOC + OUTDR) ^= (Pin(PIN3) | Pin(PIN5));
    }
}
