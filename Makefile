CFLAGS  = -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS = -Tlink.ld
ASFLAGS = -felf

KERNEL  = kernel
IMAGE   = floppy.img

SOURCES = boot.o main.o

all: $(SRC) link
	./boot/create-image.bash $(KERNEL) $(IMAGE)

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<

run:
	bochs -f bochsrc

clean:
	rm *.o kernel

.PHONY: clean run
