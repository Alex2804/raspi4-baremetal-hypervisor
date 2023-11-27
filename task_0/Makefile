CC = aarch64-none-elf-gcc
LD = aarch64-none-elf-ld
OBJCOPY = aarch64-none-elf-objcopy
QEMU = qemu-system-aarch64

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

all: clean kernel8.img

start.o: start.S
	$(CC) $(CFLAGS) -c start.S -o start.o

%.o: %.c
	$(CC) -I include $(CFLAGS) -c $< -o $@

kernel8.img: start.o $(OBJS)
	$(LD) -nostdlib start.o $(OBJS) -T link.ld -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clean-immediate:
	rm kernel8.elf *.o src/*.o > /dev/null 2>/dev/null || true

clean: clean-immediate
	rm kernel8.img > /dev/null 2>/dev/null || true

run: all
	$(QEMU) -s -semihosting -M raspi3b -kernel kernel8.img -serial null -serial stdio
	rm kernel8.img *.o src/*.o > /dev/null 2>/dev/null || true
	make clean
