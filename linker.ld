OUTPUT_FORMAT("elf32-i386")
ENTRY (loader)
KERNEL_OFFSET   = 0xC0000000;  /* 3G */
KERNEL_PHYADDR  = 0x00100000;  /* 1M */
KERNEL_VIRADDR  = KERNEL_OFFSET +  KERNEL_PHYADDR; /* 3G 1M */

SECTIONS{
	. = KERNEL_PHYADDR;
    .mbh.text ALIGN(0x4): {
        *(.mbh.text)
    }
    . += KERNEL_OFFSET;
    .text   ALIGN(0x4) : AT(ADDR(.text) - KERNEL_OFFSET) {
		*(.text)
	}
	.rodata	ALIGN (0x1000) : AT(ADDR(.rodata) - KERNEL_OFFSET) {
		*(.rodata)
	}
	.data	ALIGN (0x1000) : AT(ADDR(.data) - KERNEL_OFFSET) {
		*(.data)
	}
	.bss : AT(ADDR(.bss) - KERNEL_OFFSET) {
		sbss = . ;
		*(COMMON)
		*(.bss)
		ebss = . ;
	}

    .pgdir.data   ALIGN   (0x01000) : AT(ADDR(.pgdir.data) - KERNEL_OFFSET) {
        *(.pgdir.data)
    }
    .pgtbl.data   ALIGN   (0x01000) : AT(ADDR(.pgtbl.data) - KERNEL_OFFSET) {
        *(.pgtbl.data)
    }
	_end = . ;
}
