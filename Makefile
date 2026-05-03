CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

CFLAGS = -Wall -O2 -ffreestanding -nostdlib -Iinclude
LDFLAGS = -T linker.ld

OBJS = boot.o kernel.o uart.o gpio.o timer.o i2c.o spi.o mailbox.o framebuffer.o

all: kernel8.img

%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o : %.S
	$(CC) $(CFLAGS) -c $< -o $@

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
		-display default

clean:
	rm -f *.o kernel.elf kernel8.img

debug: CFLAGS += -g
debug: all

.PHONY: all run clean debug