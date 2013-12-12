CFLAGS=-I ./include -I ./include/boot -fno-builtin

cobjects = lib/kstdlib.o lib/printk.o src/kernel.o src/protect.o src/isr.o

objects = $(cobjects) loader.o

all: kernel

loader.o:
	nasm -f elf -o loader.o ./boot/i386/loader.asm

$(cobjects): %.o: %.c
	gcc -c $(CFLAGS) $< -o $@

kernel: $(objects)
	ld -T linker.ld -o kernel.bin $(objects)

install: kernel
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
