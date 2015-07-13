ARCH=i386

VMODE=-DKERNEL_OFFSET=0xC0000000

CFLAGS=-I . -I ./include -I ./include/boot -fno-builtin -fno-stack-protector -m32 -std=c99

GRUBLIB=/boot/grub

cobj = lib/kstdlib.o lib/kstdio.o kernel.o protect.o isr.o arch/$(ARCH)/page.o sys/pit.o arch/$(ARCH)/timer.o arch/$(ARCH)/io.o
asmobj = boot/$(ARCH)/loader.o interrupt.o
objects = $(cobj) $(asmobj)

all: kernel.bin

$(asmobj): %.o: %.asm
	nasm $(VMODE) -f elf -o $@ $<

$(cobj): %.o: %.c
	gcc -c $(VMODE) $(CFLAGS) $< -o $@

kernel.bin: $(objects)
	ld -T linker.ld -melf_i386 --cref -Map kernel.map -o $@ $^

install: clean kernel.bin
	sudo losetup	-o 32256 /dev/loop0 disk.img
	sudo mkfs.ext2 /dev/loop0
	sudo mount -o loop /dev/loop0 /mnt
	sudo mkdir -p /mnt/boot/grub
	sudo cp $(GRUBLIB)/stage1 $(GRUBLIB)/e2fs_stage1_5 $(GRUBLIB)/stage2 /mnt/boot/grub/
	sudo cp boot/grub/grub.conf /mnt/boot/grub
	sudo cp boot/grub/grub.conf /mnt/boot/grub/menu.lst
	sudo cp kernel.bin /mnt/
	sudo umount /mnt
	sudo losetup -d /dev/loop0
	sudo cat boot/grub/device-map | /sbin/grub --device-map=/dev/null

clean:
	rm -rf $(objects)
	rm -rf kernel.bin
