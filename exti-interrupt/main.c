#include <stdint.h>

#include "../include/common.h"
#include "../include/pfic.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/exti.h"
#include "../include/afio.h"

#define EXTI7_0_IRQn 20

#define PIN1 1
#define PIN3 3
#define PIN5 5

volatile uint8_t onoff[] = {0, 1};

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

void EXTI7_0_IRQHandler() __attribute__((interrupt));

void EXTI7_0_IRQHandler(){
}

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);

}

void prog(){

    flash_to_ram();

    REG(RCC + APB2PCENR) = IOPCEN | AFIOEN;

    REG(GPIOC + CFGLR) &= Clear(PIN1);
    REG(GPIOC + CFGLR) |= Input_PUPD(PIN1);

    REG(GPIOC + CFGLR) &= Clear(PIN3);
    REG(GPIOC + CFGLR) |= Output_PP_2MHz(PIN3);

    REG(GPIOC + CFGLR) &= Clear(PIN5);
    REG(GPIOC + CFGLR) |= Output_PP_2MHz(PIN5);

    REG(AFIO + EXTICR) = PC_Exti(PIN1); //PC1 as external interrupt
    REG(EXTI + INTENR) = Pin(PIN1);
    REG(EXTI + FTENR) = Pin(PIN1);  // falling edge trigger

    REG(PFIC + IENR0) = 1<<EXTI7_0_IRQn;

    while(1){
        __asm__ volatile("wfi");
        REG(EXTI + INTFR) = Pin(PIN1);
        REG(GPIOC + BSHR) = Pin(PIN3 + 16*onoff[0]);
        onoff[0] = 1 - onoff[0];
        REG(GPIOC + BSHR) = Pin(PIN5 + 16*onoff[1]);
        onoff[1] = 1 - onoff[1];
    }
}
