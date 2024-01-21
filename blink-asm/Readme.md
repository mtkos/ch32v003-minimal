# the blink-asm program

This is the same program as blink-small, only in assembly code. There are several interesting points. Instead of #define we use .set assembler directives. With #define the compiler can
do very little arithmetic. On the other hand the assembler *can* evaluate an expression like (CFGLR_ResetValue & PIN\_CFGLR\_Clear) | Pin\_Output\_PP\_2MHz at compile time. Looking at the
disassembled code (with make objdump) we see no 'and' or 'or' instructions. On the other hand, whereas the original program has 13 instructions, the assembled code has 14.
The reason for this is that an instruction like 'li t0, RCC' is not a real instruction. This would be impossible: instructions have a size of at most 32 bits. The value RCC=0x40021000 already has
32 bits, so it can cannot possibly fit in one instruction. The 'li' instruction is a so-called pseudo instruction, see [Programming], section 5.4, which the assembler expands into one or more real instructions.
Pseudo instructions are there to make programmers' lives easier. 
