.set MIE, 1<<3
.set MODE1, 1<<1
.set MODE0, 1<<0

.section .init

  la sp, _estack

  li t0, MODE1 | MODE0
  csrw mtvec, t0

  li t0, MIE
  csrw mstatus, t0

  jal prog
