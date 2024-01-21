# the blink-small program

This is the program to start with. It blinks a LED connected to pin PC3. For readability, the program parts where register addresses and bit fields are defined
have been put in separate include files.

The steps are as follows:

1. The GPIOC peripheral's clock is enabled. The reference manual, section 3.4.7, says that this clock is set by 
setting bit 4 (IOPCEN) of the APB2 Peripheral Clock Enable Register. If you do not do this you will not be able to read and write the
GPIOC registers. Compare it to an external chip, which, usually, has a clock pin to clock bits in and out. By default, the GPIOC clock is off,
probably to save power. Reading and writing peripheral registers is done using the REG macro, defined in common.h.

2. PC3 is configured by first clearing its MODE and CNF bits in Port Configuration Register Low, leaving the other bits intact, and then setting its CNF bits to 00 (Universal push-pull output mode) 
and mode bits to 10 (Output mode, maximum speed 2MHz). This is done by two macros defined in the gpio.h include file. 
Instead of 2MHz, 10 or 50 MHz will also work. The Port Configuration Register Low is documented in section 7.3.1.1 of the reference manual.

3. Then the program enters an infinite loop. The program repeatedly counts down from 100000 to 0, creating a delay, and then toggles the PC3 bit in the output register
of the GPIOC peripheral.

This program consists of only a single function, prog, with no global variables. We can just compile it using gcc and convert the resulting .elf file into an executable .bin.
The CPU will start from the instruction at flash address 0, which is also the start address off the prog function. Usually, the main function of a C program has to be called 'main'.
But, in this simple case, every name will do, as long as the start address of the function is at address 0. Observe what is NOT needed for such a simple program: startup code,
a vector table and a linker script. Later on we will see how they come into play. I did include an (empty) linker script, Link.ld, in order to suppress a warning from the linker.
