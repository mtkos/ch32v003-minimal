#define AFIO 0x40010000
#define   PCFR1 0x4
#define   EXTICR 0x8
#define     PC_Exti(x) (0b10<<(2*(x)))
