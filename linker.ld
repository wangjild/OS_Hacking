ENTRY (loader)
SECTIONS{
	. = 0x00100000;
	.text :{
		*(.text)
	}
	.rodata	ALIGN	(0x1000) : {
		*(.rodata)
	}
	.data	ALGIN	(0x1000) : {
		*(.data)
	}
	.bss : {
		sbss = .;
		*(COMMON)
		*(.bss)
		ebss = .;
	}
}
