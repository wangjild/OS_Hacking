;%include "boot/i386/gdt.asm"

global	loader
extern	kernel_main

MODULEALIGN	equ	1<<0
MEMINFO	equ	1<<1
FLAGS	equ	MODULEALIGN | MEMINFO

MAGIC	equ	0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

section .mbh.text
align 4
MultiBootHeader:
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
TrickGdts:
    ;;Gdt     0, 0, 0
    ;;Gdt     0x0 - KERNEL_OFFSET, 0xFFFFFFFF, 0x409B
    ;;Gdt     0x0 - KERNEL_OFFSET, 0xFFFFFFFF, 0x4093
    dd 0, 0
    db 0xFF, 0xFF, 0, 0, 0, 10011010b, 11001111b, 0x40  ; code selector 0x08: base 0x40000000, limit 0xFFFFFFFF, type 0x9A, granularity 0xCF
    db 0xFF, 0xFF, 0, 0, 0, 10010010b, 11001111b, 0x40  ; data selector 0x10: base 0x40000000, limit 0xFFFFFFFF, type 0x92, granularity 0xCF

TrickGdtEnd:
tgdtptr:
    dw  TrickGdtEnd - TrickGdts - 1
    dd  TrickGdts

; reserve initial kernel stack space 
section .text
STACKSIZE equ 0x4000              ; that's 16k.
loader:
    lgdt    [tgdtptr]
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    jmp     0x08:higherhalt
higherhalt:
    mov esp, stack+STACKSIZE      ; set up the stack
    finit                         ; initialize FPU
    mov	eax,	0x2BADB002
    push eax                      ; pass Multiboot magic number
    push ebx                      ; pass Multiboot info structure
    call kernel_main              ; call kernel proper

hang:
   hlt                            ; halt machine should kernel return
   jmp  hang


global gdt_flush
extern gdtptr
extern kernel_code_selector
extern kernel_data_selector
gdt_flush:
    lgdt [gdtptr]
    xor eax, eax
    mov ax, [kernel_data_selector]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax

    mov ax, [kernel_code_selector]
    mov word[SWITCH_TO + 5], ax
SWITCH_TO:
    jmp 0x0:flush2 ; Far jump! 
flush2:
    ret 

section .bss
align 32
stack:
   resb STACKSIZE      ; reserve 16k stack on a quadword boundary
