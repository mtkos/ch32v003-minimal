## common.h

This file defines one macro, REG. It takes an unsigned integer x and returns the value stored at memory location x. Here x will always be the address of a peripheral
register. The keyword 'volatile' is used to tell the compiler to avoid certain optimizations, because a register value may have been  changed by hardware, without the compiler 'knowing'
about this possibility. The usual solution for accessing registers is defining structs having a peripheral base address as address, and the 
peripherals as fields. This scales better and avoids name clashes when two peripherals have registers with the same name. Nevertheless, for ease of exposition, I have avoided this
syntactic sugar.

## rcc.h

This file defines register addresses of the RCC peripheral, chapter 3 of the reference manual. The peripheral has a base address 0x40011000. We use three registers,
CTLR, CFGR0 and APB2PCENR. They are defined by their offset with respect to the base register. To set bit 4 of the APB2PCENR register we would write
REG(RCC + APB2PCENR) |= 1<<4 . The bits have been given names. For instance IOPCEN = 1<<4 is a name for this fourth bit. The names are those from the reference manual.
The hierarchy peripheral -> register -> bit is highlighted by indentation. HPRE\_Reset\_Mask is used for clearing the HPRE bits in CFGR0, which define the prescaling
factor used to obtain the AHB clock from the system clock. When these bits are cleared both clocks have the same frequency. The AHB clock is used, for instance, by the USART
peripheral.

## other files

The other files give names for the registers and bit fields of other peripherals, in a similar way as for RCC. Two of the peripherals, SysTick and the PFIC interrupt controller are
documented in the microprocessor manual [QingKeV2].
