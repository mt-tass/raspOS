CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy


CFLAGS = -Wall -O2 -ffreestanding -nostdlib
LDFLAGS = -T linker.ld

OBJS = boot.o kernel.o uart.o

all: kernel8.img

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

kernel.o: kernel.c uart.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

uart.o: uart.c uart.h
	$(CC) $(CFLAGS) -c uart.c -o uart.o

kernel.elf: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.elf

kernel8.img: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel8.img
	@echo "Build done"
	@ls -lh kernel8.img

run: kernel8.img
	qemu-system-aarch64 \
		-M raspi3b \
		-kernel kernel8.img \
		-serial null \
		-serial stdio \
		-display none

clean:
	rm -f *.o kernel.elf kernel8.img

debug: CFLAGS += -g
debug: all

.PHONY: all run clean debug
