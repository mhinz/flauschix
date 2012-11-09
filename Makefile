CC      = gcc
CFLAGS  = -Isrc -g -std=c99 -pedantic -Wall -Wextra -Werror -nostdlib -nostdinc -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector
LDFLAGS = -Tkernel.ld
ASFLAGS = -felf

KERNEL  = kernel.bin
IMAGE   = floppy.img

SOURCES = \
	  src/boot.asm \
	  src/main.c
OBJ     = $(addsuffix .o,$(basename $(SOURCES)))

$(IMAGE): $(KERNEL)
	@./boot/create-image.bash $^ $@

$(KERNEL): $(OBJ)
	@echo '[*] Linking object files into kernel ELF binary...'
	@ld $(LDFLAGS) -o $@ $^

%.o: %.c
	@echo '[*] Compiling C files...'
	@$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.asm
	@echo '[*] Compiling ASM files...'
	@nasm $(ASFLAGS) -o $@ $^

bochs:
	@LD_PRELOAD=/usr/lib/i386-linux-gnu/libXpm.so.4 bochs -f bochsrc

clean:
	rm $(OBJ) $(KERNEL)

.PHONY: clean bochs $(IMAGE)
