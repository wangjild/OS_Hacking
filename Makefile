ARCH=i386

VMODE=-DKERNEL_OFFSET=0x00000000

CFLAGS=-I . -I ./include -I ./include/boot -fno-builtin

cobj = lib/kstdlib.o lib/kstdio.o src/kernel.o src/protect.o src/irpts.o arch/$(ARCH)/page.o
asmobj = boot/$(ARCH)/loader.o arch/$(ARCH)/irpts.o
objects = $(cobj) $(asmobj)

all: kernel.bin

$(asmobj): %.o: %.asm
	nasm $(VMODE) -f elf -o $@ $<

$(cobj): %.o: %.c
	gcc -c $(VMODE) $(CFLAGS) $< -o $@

kernel.bin: $(objects)
	ld -T linker.ld --cref -Map kernel.map -o $@ $^

install: kernel.bin
	sudo losetup	-o 32256 /dev/loop0 disk.img
	sudo mkfs.ext2 /dev/loop0
	sudo mount -o loop /dev/loop0 /mnt
	sudo mkdir -p /mnt/boot/grub
	sudo cp /boot/grub/stage1 /boot/grub/e2fs_stage1_5 /boot/grub/stage2 /mnt/boot/grub/
	sudo cp boot/grub/grub.conf /mnt/boot/grub
	sudo cp boot/grub/grub.conf /mnt/boot/grub/menu.lst
	sudo cp kernel.bin /mnt/
	sudo umount /mnt
	sudo losetup -d /dev/loop0
	sudo cat boot/grub/device-map | grub --device-map=/dev/null

clean:
	rm -rf $(objects)
	rm -rf kernel.bin
