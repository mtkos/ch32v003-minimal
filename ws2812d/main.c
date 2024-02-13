#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/dma.h"
#include "../include/gpio.h"
#include "../include/timer.h"

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);
}

#define BIT0 2 //8MHzclock -> 0.25 us high
#define BIT1 8 //1 us high
#define BIT_RESET 0
#define CNT_MAX 10 //1.25 us pulse length
#define NUM_LEDS 3
#define SIZE NUM_LEDS*24 + 1

uint8_t bits[SIZE];

void set_pixel(int led, uint32_t rgb){

    uint32_t bit;
    int i;

    i = 24*led;
    for(bit = 1<<23; bit; bit >>= 1)
        bits[i++] = (rgb & bit) ? BIT1 : BIT0;
}

#define PIN0 0

void init_clocks(){

    REG(RCC + APB2PCENR) = IOPCEN;
    REG(RCC + APB1PCENR) = TIM2EN;
    REG(RCC + AHBPCENR) = DMA1EN;
}

void dma_init(){

    REG(DMA1 + PADDR2) = TIM2 + CH3CVR;
    REG(DMA1 + MADDR2) = (uint32_t)bits;
    REG(DMA1 + CFGR2) = PSIZE16 | MINC | DIR;
}

void timer_init(){

    REG(GPIOC + CFGLR) &= Clear(PIN0);
    REG(GPIOC + CFGLR) |= Multiplexed_PP_2MHz(PIN0);

    REG(TIM2 + CCER) = CC3E;
    REG(TIM2 + CHCTLR2) = OC3M_2 | OC3M_1 | OC3PE;

    REG(TIM2 + ATRLR) = CNT_MAX;

    REG(TIM2 + DMAINTENR) = UDE;
    REG(TIM2 + CTLR1) = CEN;
}

void send_bits(){

    REG(DMA1 + CFGR2) &= ~(EN);
    REG(DMA1 + CNTR2) = SIZE;
    REG(DMA1 + CFGR2) |= EN;
}

uint32_t colors[NUM_LEDS] = {0x000808, 0x080008, 0x080800};

void cycle(){

    uint32_t c = colors[0];
    for(int i = 0; i < NUM_LEDS - 1; i++)
        colors[i] = colors[i + 1];
    colors[NUM_LEDS - 1] = c;
}

void prog(){

    flash_to_ram();
    init_clocks();
    dma_init();
    timer_init();

    while(1){
        for(int i = 0; i < NUM_LEDS; i++)
            set_pixel(i, colors[i]);
        send_bits();
        cycle();
        for(volatile int i = 50000; i; i--);
    }
}
