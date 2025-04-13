# the exti-interrupt program

This program uses a button connected to PC1 as interrupt source. When pressed, the button pulls PC1 to 0V. An interrupt is generated and the
LEDs connected to PC3 and PC5 are toggled. The configuration of PC3 and PC5 is as before. PC1 is configured as an input. To configure it as an interrupt source the necessary bits in the AFIO
(GPIO alternative fuction) EXTICR are set. Then the necessary bits in the EXTI peripheral are set so that an interrupt is generated on the falling edge on PC1. Finally, the interrupt is enabled
like in the SysTick example. The most interesting part is the interrupt routine. It does nothing but clear the interrupt flag. But in the main loop we start with the assembly 'wfi' intruction. This instruction means 'wait for
interrupt'. It makes the MCU go into low power mode. When an interrupt occurs, the MCU wakes up and starts executing instructions until the wfi instruction is executed again. So, nothing needs to
be done in the interrupt routine, except clearing the interrupt flag. Everything else is done in the main loop.

In practice, buttons need to be 'debounced'. Pressing a button always generates some electrical noise, and therefore sometimes two or more interrupts are generated very quickly one after another.
You will see that sometimes the LEDs are toggled more than once after pressing the button. Debouncing can be done in hardware (Schmitt trigger) or software.
