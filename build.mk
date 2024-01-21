PREFIX = riscv64-unknown-elf
OC = $(PREFIX)-objcopy
CC = $(PREFIX)-gcc
SZ = $(PREFIX)-size
READELF = $(PREFIX)-readelf
OBJDUMP = $(PREFIX)-objdump
NM = $(PREFIX)-nm

FLAGS = -Os -mabi=ilp32e -nostdlib -ffreestanding -Wall -Wl,-Map,$(TARGET).map

all: $(TARGET).elf

$(TARGET).elf: $(SRC)
	$(CC) $(FLAGS) -o $@ $^

$(TARGET).bin: $(TARGET).elf
	$(OC) -O binary $^ $@

clean:
	$(RM) *.elf
	$(RM) *.bin
	$(RM) *.map
	$(RM) *.dmp
	$(RM) *.html

bin: $(TARGET).bin

load: $(TARGET).bin
	../openocd -f ../wch-riscv.cfg -c "program $(TARGET).bin reset exit"

edit:
	nano main.c

touch:
	touch $(SRC)

size: $(TARGET).elf
	$(SZ) --format=SysV $^

readelf: $(TARGET).elf
	$(READELF) $^ -a

objdump: $(TARGET).elf
	$(OBJDUMP) -D > $(TARGET).dmp $^

nm: $(TARGET).elf
	$(NM) $^
