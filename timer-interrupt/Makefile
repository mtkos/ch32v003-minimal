TARGET=timer-interrupt

SRC = main.c
SRC += startup.s

include ../build.mk

FLAGS += -march=rv32e_zicsr
FLAGS += -Wl,-T../ld/Link.ld
