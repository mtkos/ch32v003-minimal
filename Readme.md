## ch32v003-minimal

When starting with microcontrollers it is quite easy to get overwhelmed by the accompanying software. Thousands of lines of code: makefiles, 
linker scripts, assembly startup code, vector tables .... Trying to understand them I wrote a series of toy programs, which, I hope, show that all of these are quite
well documented and not that complicated to understand. I added a lot of explanation, hoping that it may be useful for those who
have the questions I had. The microprocessor I used is the fairly new ch32v003 RISC-V MCU. No software other than the GNU compiler and binutils has been used, except for the last
program, which also uses picolibc.

Everything has been tested on Debian 12 (bookworm, GCC 12.2) and Debian 13 (trixie, GCC 12.2). The programs have been 'optimized' for ease of understanding and reading. This has led
to some choices which would be impractical on a larger scale. For instance, the names of peripheral registers are those of the MPU manual, without the prefix denoting the peripheral
to which it belongs. So CTLR instead of RCC\_CTLR. But there is also WWDG\_CTLR and FLASH\_CTLR. This works because I only use the first of these.

Interestingly, the resulting binaries
are quite small: only 112 byte for a program which blinks a LED, and even 80 bytes if compressed instructions are allowed. This is only a little more than the size obtained when writing
the program in assembly (50 bytes), and certainly a lot less than what you get from toolchains like MounRiver Studio.

### Remarks

The following documentation has been used:

[QingKeV2] The QingKeV2 Microprocessor Manual, can be obtained from <https://www.wch-ic.com/downloads/file/369.html>.

[CH32V003] The CH32V003 Reference Manual, can be obtained from <https://www.wch-ic.com/downloads/CH32V003RM_PDF.html>. We will refer to it simply as
'The reference manual'.

[Programming] [An Introduction to Assembly Programming with RISC-V](https://riscv-programming.org/book/riscv-book.html), especially chapters 4, 5 and 9.

[As] [The GNU Assembler manual](https://ftp.gnu.org/old-gnu/Manuals/gas-2.9.1/html_chapter/as_toc.html).

[GNU-linker] [The GNU linker](https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_toc.html), especially the Command Language chapter.

[Picolibc] [Picolibc](https://github.com/picolibc/picolibc)

Apart from these, a little knowledge of the C programming language is needed.

### The programs

The first programs are very simple. New concepts are introduced gradually.

1. blink-small It blinks a LED and does not use anything special: no startup code, no linker script, no vector table.

2. blink-asm The same, only in assembly language.

3. uart Introduces a new peripheral, the USART, which allows you to communicate with the MCU via a serial line.

4. blink It blinks two LEDs. It uses global variables and shows how a linker script determines where data is located in FLASH and RAM, and how you, the programmer, have to take care
that read-write data is transferred from FLASH to RAM at program startup.

5. systick-interrupt Again blinks a LED, but this time using the systick timer core peripheral. It introduces interrupts: how to write an interrupt routine and how to enable interrupts.
It is the first program where a vector table is needed and a very small piece of startup code that is best done in assembly.

6. exti-interrupt The same as systick-interrupt, but this time using the external interrupt from a button. It introduces the wfi (wait for interrupt instruction) which puts the MCU in low
power mode. The interrupt routine does nothing, it only wakes up the MCU. Everything else, including clearing the interrupt flag, is done outside of the interrupt routine.

7. timer A program that uses the timer peripheral to blink a LED. It shows how the timer keeps running while the CPU goes to sleep.

8. timer-interrupt The same as 5. and 6., but now using one of the timer peripherals. It uses the 'update' interrupt, triggered when the timer counter overflows.

9. picolibc-example Shows how to use the picolibc library. Using it you can have stdio functionality like printf in your programs and use mathematical functions like sin and cos, among other
things.

10. ws2812d uses the DMA controller and PWM to control a chain of ws2812d LEDs.

### Prerequisites

You will need a compiler: Debian packages gcc-riscv64-unknown-elf and binutils-riscv64-unknown-elf. And the GNU make utility, Debian package 'make'. To use the picolibc library Debian offers a
prebuilt version in package picolibc-riscv64-unknown-elf.

To run the programs you need to upload them to the MCU. You can use the loader you like. I use the WCH-LinkE debugger/uploader, together with the openocd version from the MounRiver 
Toolchain & Debugger \(<http://www.mounriver.com/download>\). For copyright reasons I have not included MounRiver files here. Look for the file named openocd, and copy it to the dummy openocd 
in the base directory of this project (where this Readme also resides). No need to use the MounRiver installer script, which could be dangerous. Instead: look for files
50-wch.rules and 60-openocd.rules and copy them (as root) to /etc/udev/rules.d. Then add yourself, if necessary, to the plugdev group, log out and log in again. Install, if necessary, Debian packages 
libhidapi-hidraw0, libhidapi-usb0 and libjaylink0. Not sure whether all of these are necessary. On non-debian systems they may have other names. The openocd configuration file, wch-riscv.cfg,
should also be put in the base directory.

The WCH-LinkE also provides power for your MCU. And it has a built-in serial converter. You will have a device, probably called /dev/ttyACM0, through which you can read and write data to the
uart. Use any serial console you like: screen,  PuTTY, miniterm from pyserial or whatever you prefer. In some cases you may have to add yourself to, for instance, the dialout group
(use ls -l /dev/ttyACM0 to find out which).

### The build.mk file

This file is included by the Makefile of all programs. They add their own, program specific, options. The compiler flag -ffreestanding is needed to have stdint.h available. The option -Wl,-Map
generates a .map file showing where the sections and variables are located in memory. The $(TARGET).bin target creates a .bin file which can be uploaded to the MCU using the MounRiver openocd.
The targets size, readelf, objdump and nm generate useful additional information about the compiled program. If, for instance, your linker script has a bug, causing your program to fail,
you can check if variables are in the right sections, are aligned as necessary, etc. . The separate Makefile for each program contains extra information for the compiler . For instance,
sometimes we use -march=rv32ec. But if the program uses interrupts, we need the csr instructions, and use -march=rv32ec\_zicsr. If you want to avoid compressed instructions, to prevent
alignment problems, you can use -march=rv32e\_zicsr. The Makefile also determines the linker script to use.

### Further documentation

The programs and other files are documented and explained by Readme files in their respective directories.
