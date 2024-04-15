# the timer-interrupt program

It works like the exti-interrupt program. This time, it uses the 'update' interrupt of the Timer 1 peripheral, which is generated on counter overflow. It does not use a vector table, but,
instead, the 'Vector Table Free (VTF) interrupt response mechanism', documented in chapter 3 of the processor manual [QingKeV2]. For this reason, the startup code is very simple.

