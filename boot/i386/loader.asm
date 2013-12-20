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
; reserve initial kernel stack space
 
section .text
STACKSIZE equ 0x4000              ; that's 16k.
loader:
    mov esp, stack+STACKSIZE - KERNEL_OFFSET      ; set up the stack
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
    lgdt [gdtptr-KERNEL_OFFSET]
    xor eax, eax
    mov ax, [kernel_data_selector-KERNEL_OFFSET]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax

    mov ax, [kernel_code_selector-KERNEL_OFFSET]
    mov word[SWITCH_TO + 5], ax
SWITCH_TO:
    jmp 0x0:flush2 ; Far jump! 
flush2:
    ret 

section .bss
align 32
stack:
   resb STACKSIZE      ; reserve 16k stack on a quadword boundary
