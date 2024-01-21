# the systick program

This program introduces interrupts. And it introduces a new peripheral, the systick timer. It is a core-peripheral. For that reason it is documented in the processor reference manual [QingKeV2].
Conceptually, there is no difference between core- and non core- peripherals. The only difference I see is that you do not have to start a clock to access its registers. The program blinks 2 LEDs.
One of these is toggled in the main program. The other one is toggled in the SysTick\_Handler interrupt routine. An interrupt routine interrupts normal program flow when it is triggered. It executes,
after which normal program flow continues. An interrupt is only triggered if it is *enabled*. Enabling is done through the PFIC core peripheral, by setting bit 12, the hardware-defined interrupt
number for systick, in the IENR0 register. IENR0 contains the enable bits for interrupt numbers 0 through 31. There is also IENR1 for interrupt numbers 32 through 63. In total,there are 8
interrupt enable registers, allowing for 256 interrupt sources.

In order for the interrupt routine to be executed, the processor has to know its address. This is where the *vector table* comes into play. If a systick interrupt (interrupt number 12) occurs,
the processor looks up that address at word number 12 of the vector table. By default, the vector table starts at address 0x00000000 in FLASH. The vector table is created in the source file
startup.s. Because we use only one interrupt we only fill the corresponding entry in the table, leaving the remaining ones undefined. The first entry of the vector table does not contain the
address of an interrupt routine, but a jump instruction. This instruction is executed at program start. In our case it is a jump to the Reset_Handler label. Any other name will also do, of course.
The Reset\_Handler routine contains some startup code. First, it loads the value \_estack, defined in the linker script, into register sp, which contains the stack pointer. Then, it sets the
MODE0 and MODE1 bits of the mtvec register. This register is described in section 3.2 of [QingKeV2]. This means that the vector table starts at address 0x00000000 and that
the vector table works as described above. The register mtvec is one of the so-called CSR registers of the processor. They require special instructions to be read or written. More information
can be found in [Programming], section 9.3. Finally, interrupts are enabled by setting the MIE bit of the mstatus CSR register, after which control is transferred to the main routine prog.
The reason that the main routine in a C program usually is called 'main' is because it is defined this way in the startup code. But, in fact, you can name it as you like it. The startup
routine is placed at the start of FLASH because its section, .init, comes first in the linker script.

Usually, the code which initializes SRAM (our flash\_to\_ram) is also in the startup program. There is no real reason for this, and in C it is much more readable.

Interrupt routines need special handling by the compiler. For that reason it is important not to forget the 'interrupt' attribute. Leave it out and you will see that the program does not behave as
expected anymore.

The systick peripheral is documented in section 4 of [QingKeV2]. Because we do not configure the system clock, it runs at default 8MHz. Because the STCLK bit in the systick control register is not
set, the systick clock runs at 1MHz. Because the counter reload value is 1000000, a systick interrupt occurs once every second.

It is very easy to make mistakes handling interrupts. You can easily forget one of the following:

1. Set the mstatus MIE bit.

2. Set the necessary bit in IENRx (x = 0, 1, ...)

3. Enable the peripheral interrupt, in our case set the STIE bit

4. Start the peripheral, in our case, set the STE bit. If the counter does not run, it will not reach the reload value, and no interrupt will be triggered.
