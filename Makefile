CC      := gcc

LDFLAGS := -Tkernel.ld -melf_i386
ASFLAGS := -felf
#CFLAGS  := -Isrc/kernel -Isrc/libc -g -std=c99 -pedantic -Wall -Wextra -Werror -nostdlib -nostdinc -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized -Wconversion -Wstrict-prototypes
CFLAGS  := -Isrc/kernel -Isrc/libc -g -m32 -std=c99 -pedantic -Wall -Wextra -Werror -nostdlib -nostdinc -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized -Wconversion

KERNEL  := kernel.bin
IMAGE   := floppy.img

SRC     := $(shell find src -type f -name '*.c' -o -name '*.asm')
OBJ     := $(addsuffix .o,$(basename $(SRC)))

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
	@qemu -kernel $(KERNEL)

clean:
	rm -f $(OBJ) $(KERNEL)

mrproper: clean
	rm -f $(IMAGE) bochs.log

.PHONY: mrproper clean bochs qemu $(IMAGE)
