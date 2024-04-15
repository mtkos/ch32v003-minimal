#include <stdint.h>

#include "../include/common.h"
#include "../include/pfic.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/timer.h"

#define TIM1_UP_IRQn 35

#define PIN3 3

#define CPU_FREQ 8000000
#define CNT_MAX 0xffff
#define PRESCALER_VALUE (CPU_FREQ/CNT_MAX - 1)

void TIM1_UP_IRQHandler() __attribute__((interrupt));

void TIM1_UP_IRQHandler(){
}

void prog(){

    REG(RCC + APB2PCENR) = TIM1EN | IOPCEN;

    REG(GPIOC + CFGLR) &= Clear(PIN3);
    REG(GPIOC + CFGLR) |= Output_PP_2MHz(PIN3);

    REG(TIM1 + PSC) = PRESCALER_VALUE;

    REG(TIM1 + DMAINTENR) = UIE;
    REG(PFIC + IENR1) = (1<<(TIM1_UP_IRQn % 32));
    REG(PFIC + VTFIDR) = TIM1_UP_IRQn;
    REG(PFIC + VTFADDRR0) = (uint32_t)TIM1_UP_IRQHandler + 1;

    REG(TIM1 + CTLR1) = CEN;

    while(1){
        __asm__ volatile ("wfi");
        REG(TIM1 + INTFR) = 0;
        REG(GPIOC + OUTDR) ^= Pin(PIN3);
    }
}
