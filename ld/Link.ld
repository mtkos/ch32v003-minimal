MEMORY{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 16K
	RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 2K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS{
    .init :{ 
      . = ALIGN(4);
      *(.init)
      . = ALIGN(4);
    } >FLASH

    .text :{
      . = ALIGN(4);
      *(.text .text*)
      *(.rodata .rodata*)
      *(.srodata .srodata*)
      . = ALIGN(4);
    } >FLASH

    .sdata :{
      . = ALIGN(4);
      _sdata = .;
    } >RAM

    .sidata :{
      . = ALIGN(4); 
      _sidata = .;
    } >FLASH

    .data :{
      . = ALIGN(4);
      *(.data .data.*)
      *(.sdata .sdata.*)
      . = ALIGN(4);
      _edata = .;
    } >RAM AT>FLASH

    .bss :{
      . = ALIGN(4);
      _sbss = .;
      *(.sbss*)
      *(.bss*)
      . = ALIGN(4);
      _ebss = .;
    } >RAM
}
