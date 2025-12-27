CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

CFLAGS = -Wall -O2 -ffreestanding -nostdlib 
LDFLAGS = -T linker.ld

OBJS = boot.o kernel.o uart.o gpio.o timer.o i2c.o spi.o mailbox.o

all: kernel8.img

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

kernel.o: kernel.c uart.h gpio.h timer.h i2c.h spi.h mailbox.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

uart.o: uart.c uart.h
	$(CC) $(CFLAGS) -c uart.c -o uart.o

gpio.o: gpio.c gpio.h
	$(CC) $(CFLAGS) -c gpio.c -o gpio.o

timer.o : timer.c timer.h
	$(CC) $(CFLAGS) -c timer.c -o timer.o

i2c.o : i2c.c i2c.h
	$(CC) $(CFLAGS) -c i2c.c -o i2c.o

spi.o : spi.c spi.h
	$(CC) $(CFLAGS) -c spi.c -o spi.o

mailbox.o : mailbox.c mailbox.h
	$(CC) $(CFLAGS) -c mailbox.c -o mailbox.o

kernel.elf: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.elf

kernel8.img: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel8.img
	@echo "Build complete!"
	@ls -lh kernel8.img

run: kernel8.img
	@echo "Starting QEMU (Raspberry Pi 3B)..."
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