.set TIM1_UP_IRQn, 35

.section .init
  j      Reset_Handler
.skip    4*(TIM1_UP_IRQn - 1)
.word    TIM1_CC_IRQHandler

Reset_Handler:
  la sp, _estack

/* mtvec mode MODE1 | MODE0, vectored mode */
  li t0, 0x3
  csrw mtvec, t0

/* set MIE bit in mstatus, enable interrupts*/
  li t0, 0x8
  csrw mstatus, t0

  jal prog
