.set EXTI7_0_IRQn, 20
.set MIE, 1<<3
.set MODE1, 1<<1
.set MODE0, 1<<0

.section .init
  j      Reset_Handler
.skip    4*(EXTI7_0_IRQn - 1)
.word    EXTI7_0_IRQHandler

Reset_Handler:
  la sp, _estack

  li t0, MODE1 | MODE0
  csrw mtvec, t0

  li t0, MIE
  csrw mstatus, t0

  jal prog
