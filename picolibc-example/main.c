#include <stdio.h>
#include <math.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/uart.h"

#define BAUDRATE 921600
#define CLOCK_FREQUENCY 48000000
#define BAUD_REG CLOCK_FREQUENCY/BAUDRATE - 1

#define PIN5 5
#define PIN6 6

int uart_putc(char c, FILE *file){

    REG(USART1 + DATAR) = c;
    while(!(REG(USART1 + STATR) & TC));
    return c;
}

int uart_getc(FILE *file){

    unsigned char c;

    while(!(REG(USART1 + STATR) & RXNE));
    c = REG(USART1 + DATAR);
    return c;
}

FILE __stdio = FDEV_SETUP_STREAM(uart_putc, uart_getc, NULL, _FDEV_SETUP_RW);

FILE *const stdin = &__stdio;
__strong_reference(stdin, stdout);
__strong_reference(stdin, stderr);

void uart_init(){

    REG(RCC + CFGR0) &= HPRE_Reset_Mask;
    REG(RCC + CFGR0) |= HPRE_DIV1;

    REG(RCC + CTLR) |= PLLON;
    while(!(REG(RCC + CTLR) & PLLRDY));
    REG(RCC + CFGR0) |= SW_PLL;

    REG(RCC + APB2PCENR) = USART1EN | IOPDEN;

    REG(GPIOD + CFGLR) &= Clear(PIN5);
    REG(GPIOD + CFGLR) |= Multiplexed_PP_2MHz(PIN5);

    REG(GPIOD + CFGLR) &= Clear(PIN6);
    REG(GPIOD + CFGLR) |= Input_PUPD(PIN6);

    REG(USART1 + BRR) = BAUD_REG;
    REG(USART1 + CTLR1) = TE | RE | UE;
}

void main(){

    uart_init();

    while(1){
        getchar();
        printf("%.15f\r\n", 4.0*atan(1.0));
    }
}
