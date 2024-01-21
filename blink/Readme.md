# the blink program

This is the first program which needs a linker script. The program in itself is simple. It blinks two LEDs, in a way which should by now be familiar. But the main difference is the
presence of a global variable 'count', which determines the blinking frequency. When a program is compiled, the resulting object file consists of so-called 'sections'. Roughly speaking,
there are three of these: section .text, consisting of instructions, section .data, consisting of initialized data, and section .bss, consisting of uninitialized data. The .text section
is in FLASH memory: the CPU reads instructions from FLASH. The initialized data are also in FLASH memory. But these data may be modified during program execution. FLASH memory *can* be
read and written. However, writing is an expensive operation. The memory has to be written, not in bytes, but in pages, which, in our case are of size 64 byte. Furthermore, writing FLASH
cannot be done too often, because FLASH is prone to wear: it can be written maybe a 100.000 times but not much more. For this reason, we have to consider FLASH as read-only memory.
The ch32v003 has 16kB of FLASH. It also has 2kB of SRAM. SRAM memory is volatile, so it cannot be used to store a program permanently. On the other hand, it can be written, fast. You may compare
FLASH to your computer's hard disk drive and SRAM to memory. This is were the linker script comes into play. It describes where the .text section will be placed in FLASH, and where the .data section
gets placed. In FLASH, but also in SRAM. The details are described in [GNU-linker], especially the Command Language chapter. Of course, data is not copied to SRAM from FLASH by magic.
You have to do it yourself. This is what the flash\_to\_ram function does. Apart from initialized data, there is uninitialized data. It goes into the .bss section. This is to save space in FLASH.
For .bss data no values have to be stored in FLASH. By convention, .bss data is initialized to zero in RAM. However, this is only a convention. You could omit the corresponding line in the
flash\_to\_ram function, but then you should not count on uninitialized values being set to 0 at program startup. It is all in your own hands. The flash\_to\_ram function needs five values which
are defined in the linker script Link.ld. They are: \_sidata, the address in FLASH where initialized data begins, \_sdata and \_edata, the addresses between which the initialized data
lands in SRAM, and \_sbss and \_ebss, for uninitialized data.

Something more has to be said about the linker script. We want execution to start at the prog function, and not, for instance, at the period function. In the Makefile you will see the flag
-ffunction-sections. This makes the compiler generate a separate .text section for each function. For instance, the code for the prog function will go in section .text.prog. The linker script
places this section first in the binary which is put in FLASH. You can verify this by inspecting the blink.map file.

Finally, in the flash\_to\_ram function you may be puzzled about &\_sdata, where you might have expected just \_sdata. This can be understood as follows. \_sdata is a *variable*, i.e., a name for
a memory location. In this case, both the count variable and \_sdata are names for the same memory location, as you can see from blink.map. Or, you can inspect the symbol table with the command
make nm. So, the value of \_sdata is the same as the value of count. We really want &\_sdata. Indeed, if you like to write hard-to-understand programs, you could replace count by \_sdata in
the period function and the program will keep working as before.
