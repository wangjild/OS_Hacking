all:
	sudo nasm -o boot.bin	boot_floppy.asm
	sudo dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp	loader.bin /mnt/floppy/ -v
	sudo umount /mnt/floppy/
