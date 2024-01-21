#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/uart.h"

#define BAUDRATE 1200
#define CLOCK_FREQUENCY 48000000
#define BAUD_REG CLOCK_FREQUENCY/BAUDRATE - 1

#define PIN5 5
#define PIN6 6

int prog(){

    REG(RCC + CFGR0) &= HPRE_Reset_Mask;
    REG(RCC + CFGR0) |= HPRE_DIV1;

    REG(RCC + CTLR) |= PLLON;
    while(!(REG(RCC + CTLR) & PLLRDY));
    REG(RCC + CFGR0) |= SW_PLL;

    REG(RCC + APB2PCENR) = USART1EN | IOPDEN;

    //PD5 = TX
    REG(GPIOD + CFGLR) &= Clear(PIN5);
    REG(GPIOD + CFGLR) |= Multiplexed_PP_2MHz(PIN5);

    //PD6 = RX
    REG(GPIOD + CFGLR) &= Clear(PIN6);
    REG(GPIOD + CFGLR) |= Input_PUPD(PIN6);

    REG(USART1 + BRR) = BAUD_REG;
    REG(USART1 + CTLR1) = TE | RE | UE;

    while(1){
        while(!(REG(USART1 + STATR) & RXNE));
        REG(USART1 + DATAR)++;
        while(!(REG(USART1 + STATR) & TC));
    }
}
