# the uart program

This program introduces a new peripheral, the USART. It should not be difficult to understand now. It starts with a section where the system clock is set to 48MHz. The internal oscillator
provides 24MHz. By default, this frequency is divided by 3 (reference manual section 3.4.2). We use the full frequency and multiply it by 2 through the PLL (phase-locked loop).
Then pins PD5 and PD6 are configured as TX and RX of the USART peripheral. The baudrate is determined by the register BRR. The description in the reference manual is unnecessarily complicated.
The baudrate is just the clock frequency, divided by (BRR + 1). The peripheral is enabled, after which the program enters an endless loop. It waits for a character to be input and outputs the ASCII
character following the one that was input. If you type 'k', 'l' will be received back. If you use the GNU screen terminal you can use the command screen /dev/ttyACM0 1200.
