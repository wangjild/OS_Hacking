global	loader
extern	kernel_main

MODULEALIGN	equ	1<<0
MEMINFO	equ	1<<1
FLAGS	equ	MODULEALIGN | MEMINFO

MAGIC	equ	0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

section .text
align 4
MultiBootHeader:
   dd MAGIC
   dd FLAGS
   dd CHECKSUM
; reserve initial kernel stack space
 
STACKSIZE equ 0x4000              ; that's 16k.
loader:
    mov esp, stack+STACKSIZE      ; set up the stack
    finit                         ; initialize FPU
    mov	eax,	0x2BADB002
    push eax                      ; pass Multiboot magic number
    push ebx                      ; pass Multiboot info structure
    call kernel_main              ; call kernel proper
    cli
hang:
   hlt                            ; halt machine should kernel return
   jmp  hang

global gdt_flush
extern gdtptr
extern kernel_code_selector
extern kernel_data_selector
gdt_flush:
    lgdt [gdtptr]
    mov ax, [kernel_data_selector]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    jmp 0x08:flush2 ; Far jump! 
flush2:
    ret 

section .bss
align 32
stack:
   resb STACKSIZE      ; reserve 16k stack on a quadword boundary
