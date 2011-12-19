; CopyRight
; FAT12 File System Booter


;=======================================
;%define	_BOOT_DEBUG

org 07c00h

jmp short _start ; 2 bits
nop		; 1 bits

;=========================================
; File System Head Here

bsOEM	db	"catos0.1"	;	fixed 8 bytes
bsSectorSize	dw	512	;	fixed	512 bytes per sector
bsClusterSize	db	1		;	current 1 sector per cluster
bsResvdSect	dw	01		;	1 resv sector
bsFatCnt	db	2				; 2 File Allocation Table
bsRootSize	dw	224		;	max num of files of root directory
bsTotalSect	dw	2880	;	max logical sector num
bsMedia	db	0xF0			;	media descriptor
bsFatSize	dw	0x09		;	size of each FAT
bsSectPerTrac	dw	18	;	sector per track
bsHeadCnt	dw	0x02		;	num of r/w head
bsHiden	dd	0					;	hidden sectors
bsHugeSect	dd	0			;	
bsBootDrv	db	0				;	driver number for int 13h
bsReservd	db	0				;	not used
bsBootSign	db	29h		;	extend booting signature
bsVolumeID	dd	0			;	disk volume id
bsVolumeLabel	db	"volume_num1"
bsFSType	db	"FAT12   ";	fixed 8 bytes

;====================================
;	Boot Code start here

stackbase	equ	07c00h	; stack base

ldbase	equ	09000h	;	base address which loader.bin will be loaded to
ldoffset	equ	0100h	;	offset of loader.bin will be loaded to

rootStart	equ	19		;	the sector num of head of root dir

_start:
	
	mov	ax,	cs
	mov	ds,	ax
	mov	es,	ax
	mov	ss,	ax
	mov	sp,	stackbase

	xor	ah,	ah
	xor	dl,	dl
	int 13h			;	reset the floppy

	call showbootmsg
	
	nop
	nop

	call	initdata	;	init

LABEL_SEARCH_LOADER_IN_ROOTDIR:
	cmp	word	[remainRootSec],	0
	jz	LABEL_LOADER_NOT_FOUND
	dec	word	[remainRootSec]

	mov	ax,	ldbase
	mov	es,	ax
	mov	bx,	ldoffset
	mov	ax,	word	[rootSecForRead]
	xor ecx,	ecx
	mov	cl,	1

	call readsec

	mov	si,	loaderfnm
	mov	di,	ldoffset
	cld
	
	mov	dx,	10h	;	512 / 32 = 16 = 10h
LABEL_SEARCH_LOADER:
	cmp	dx,	0
	jz	LABEL_NEXT_SECTOR
	dec	dx
	mov	cx,	11	;	11	bytes of DIR_Name
LABEL_CMP_FILENAME:
	cmp	cx,	0
	jz	LABEL_FILENAME_FOUND
	dec	cx
	lodsb	;
	cmp	al,	byte	[es:di]
	jz	LABEL_GOON
	jmp	LABEL_DIFF

LABEL_GOON:
	inc	di
	jmp	LABEL_CMP_FILENAME

LABEL_DIFF:
	and di,	0FFE0h
	add	di,	20h
	mov	si,	loaderfnm
	jmp	LABEL_SEARCH_LOADER

LABEL_NEXT_SECTOR:
	add word	[rootSecForRead],	1
	jmp LABEL_SEARCH_LOADER_IN_ROOTDIR
;===========================================================
;	-----------------------------
;	|    ax    |      .......   |
;	-----------------------------
;	\                           /
;  \        "cl" sectors     / 

readsec: 
	push bp
	sub	sp,	2
	mov	bp,	sp
	mov	byte	[bp],	cl

	push bx
	mov	bl,	[bsSectPerTrac]
	div	bl	;	
	inc	ah
	mov	cl,	ah
	mov	dh,	al
	shr	al,	1
	mov	ch,	al
	and	dh,	1

	mov	dl,	[bsBootDrv]
	pop bx
.keepreading:
	mov	ah,	2
	mov	al,	byte	[bp]
	int 13h
	jc	.keepreading

	add sp,	2
	pop	bp
	ret
;===========================================================
showmsg:
	mov	ax,	01301h
	mov	bx,	000ch
	int 10h
	ret

showbootmsg:
	mov	ax,	ds
	mov	es,	ax
	mov	ax,	bootmsg
	mov	bp,	ax
	mov	cx,	msglen
	mov	dx,	00000h
	call showmsg	
	ret

show_found:
	mov	ax,	ds
	mov	es,	ax
	mov	ax,	msg_ld_found
	mov	bp,	ax
	mov	cx,	msg_ld_found_len
	mov	dx,	00100h
	call showmsg
	ret

show_unfound:
	mov	ax,	ds
	mov	es,	ax
	mov ax, msg_ld_unfound
  mov bp, ax
	mov cx, msg_ld_unfound_len
	mov dx, 00100h
	call showmsg
	ret
;============================================================
initdata:
	push	ax
	push	bx
	push	dx
	xor	eax,	eax
	mov	al,	byte	[bsFatCnt]
	mul	word	[bsFatSize]
	add	ax,	1	;	caculate the begin sector of Root Dir
	mov	word	[rootSecForRead],	ax	; store

	mov	ax,	word	[bsRootSize]
	mov	bx,	32
	mul	bx

	mov	bx,	512
	div	bx
	cmp	dx,	0
	jz	.END
	add	ax,	1
.END:
	mov	word	[remainRootSec],	ax

	pop	dx
	pop	bx
	pop	ax
	ret


LABEL_LOADER_NOT_FOUND:
	call	show_unfound
	jmp	$

LABEL_FILENAME_FOUND:
	call show_found
	jmp $
;=============================================================
;	data region
;=============================================================
bootmsg	db	"Booting..."
msglen	equ	$	-	bootmsg

loaderfnm	db	"LOADER  BIN",	0	;	loader's file name

msg_ld_found	db	"Loading Loader..."
msg_ld_found_len	equ	$ - msg_ld_found

msg_ld_unfound	db	"Loader Not FOUND!"
msg_ld_unfound_len	equ	$ - msg_ld_unfound

rootSecForRead	dw	0
remainRootSec		dw	0

times	510 - ($ - $$) db	0	;	padding 0 to remain space

dw	0xaa55	;	end flag
