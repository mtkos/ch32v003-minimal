# the ws2812d program

The ws2812d is a RGB LED. These have a built-in controller, which uses a proprietary protocol. A chain of these LEDs can be controlled by sending RGB data, 8 bits per color. A 0 bit is
a 0.25 microsecond (us) high, followed by a 1 us low. A 1 bit is a 1 us high, followed by a 0.25 us low. Transmission ends after a low which is longer than 300 us.
So the bit-rate is 800000 bps, which is quite high. Nevertheless it can be achieved even at the ch32v003 default clock frequency of 8MHz. The program uses PWM. I, arbitrarily, chose
timer TIM2, channel 3, for PWM. Setting the autoreload register to 10, the counter overflows with a frequency of 800000 Hz. To send a 0 bit the TIM2 CH3CVR register has to have the value 2, and 8
for a 1 bit. These values are computed using the set\_pixel function, and stored in array 'bits'. This array is then used as the source for a DMA transfer. The destination is the TIM2 CH3CVR register.
We configure the TIM2 DMAINTENR register so that every time the counter overflows (update event), a DMA request is generated. Table 8-2 of the reference manual shows that we have to use
DMA channel 2. This channel is configured in the function dma\_init. Finally, transmission is started by setting the number of bytes to be transferred and enabling the channel. Timer 2 channel 3
outputs on pin PC0, see section 7.3.2.1 of the reference manual.
