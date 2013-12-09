CFLAGS=-I ./include -I ./include/boot -fno-builtin

kernel:
	nasm -f elf -o ./boot/i386/loader.o	./boot/i386/loader.asm
	gcc $(CFLAGS) -o lib/kstdlib.o -c lib/kstdlib.c 
	gcc $(CFLAGS) -o lib/printk.o -c lib/printk.c
	gcc $(CFLAGS) -o src/kernel.o -c src/kernel.c
	ld -T linker.ld -o kernel.bin boot/i386/loader.o lib/kstdlib.o lib/printk.o src/kernel.o

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
	find . -type f -name *.o | xargs rm -rf	
