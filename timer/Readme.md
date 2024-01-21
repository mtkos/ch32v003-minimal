# the timer program

This program shows that the MCU can go to sleep, while peripherals keep working. It uses timer 1 in PWM 1 mode, see the reference manual. The ch32v003 can use pins as timer output channel.
By default, timer 1 uses PC3 as output channel 3. The timer prescaler is chosen so that the timer counts from 0 to its maximum value 0xffff in one second. The compare value CH3CVR is
chosen such that PC3 is high for 1/10 of the 1 second cycle. Finally, the MOE bit is set, to activate output.
