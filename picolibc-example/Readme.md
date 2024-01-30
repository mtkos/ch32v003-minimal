# the picolib-example program

For writing somewhat more ambitious programs, the picolib library is very useful. It offers formatted in- and output, and mathematical functions like sin and cos, among other things.
To use it, you only have to
implement two functions, for reading and writing a character. Picolib comes with its own startup code and corresponding linker script. You only have to add some information about the memory layout.
All the magic the c compiler needs is hidden: just add the flag -specs=picolibc.specs. In Debian, picolib is contained in the picolibc-riscv64-unknown-elf package. The example program repeatedly
reads a character and then prints the value of the number pi to 15 digits. The resulting code just fits in the 16kB flash. Formatted output of floating point values takes a lot of code space. 
Although picolib is small, it is more useful on a chip like ch32v203, which has 64kB. If you can do without floating point, you can reduce code size by adding -DPICOLIBC\_INTEGER\_PRINTF\_SCANF to
your c flags.
