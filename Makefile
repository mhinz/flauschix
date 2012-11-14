CC      := gcc
CFLAGS  := -Isrc/kernel -Isrc/libc -g -std=c99 -pedantic -Wall -Wextra -Werror -nostdlib -nostdinc -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector
LDFLAGS := -Tkernel.ld
ASFLAGS := -felf

KERNEL  := kernel.bin
IMAGE   := floppy.img

SOURCES := \
	  src/kernel/boot.asm      \
	  src/kernel/common.c      \
	  src/kernel/console.c     \
	  src/kernel/gdt.c         \
	  src/kernel/gdt_flush.asm \
	  src/kernel/main.c        \
	  src/libc/string.c
OBJ     := $(addsuffix .o,$(basename $(SOURCES)))

$(IMAGE): $(KERNEL)
	@./boot/create-image.bash $^ $@

$(KERNEL): $(OBJ)
	@echo '[*] Linking object files into kernel ELF binary...'
	@ld $(LDFLAGS) -o $@ $^

%.o: %.c
	@echo '[*] Compiling C file..... $^'
	@$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.asm
	@echo '[*] Compiling ASM file... $^'
	@nasm $(ASFLAGS) -o $@ $^

bochs:
	@LD_PRELOAD=/usr/lib/i386-linux-gnu/libXpm.so.4 bochs -f emu_configs/bochsrc

qemu:
	@qemu -D log/qemu.log -kernel $(KERNEL)

clean:
	rm -f $(OBJ) $(KERNEL)

mrproper: clean
	rm -f $(IMAGE) bochs.log

.PHONY: mrproper clean bochs qemu $(IMAGE)
