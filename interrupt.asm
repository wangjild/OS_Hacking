;;
;;  Copyright (C) 2013  Wang Jing
;;
;;  This program is free software: you can redistribute it and/or modify
;;  it under the terms of the GNU General Public License as published by
;;  the Free Software Foundation, either version 3 of the License, or
;;  any later version.
;;
;;  This program is distributed in the hope that it will be useful,
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;;  GNU General Public License for more details.
;;
;;  You should have received a copy of the GNU General Public License
;;  along with this program.  If not, see <http://www.gnu.org/licenses/>.

;;   Author:        wangjild <wangjild@gmail.com>
;;   Github:        https://github.com/wangjild/OS_Hacking
;;   File:          irpts.asm
;;   Lauguage:      asm
;;   Date:          13-12-13 20:23:03
;;   Descripton:    interupts 0 - 32  entries

[BITS 32]

; int0 除0异常 
global isr0 
extern _do_divide0_error
isr0:
    cli
    push    0x00 ; 当做错误码 push 入栈
    push    _do_divide0_error
    jmp     exception_handler

; int1 单步调试
global isr1
extern _do_debug
isr1:
    cli
    push    0x00
    push    _do_debug
    jmp     exception_handler

; int2 NMI
global isr2
extern _do_nmi
isr2:
    cli
    push    0x00
    push    _do_nmi
    jmp     exception_handler

; int3 断点
global isr3
extern _do_debug_break
isr3:
    cli
    push    0x00
    push    _do_debug_break
    jmp     exception_handler
    
; int4 溢出
global isr4
extern _do_overflow
isr4:
    cli
    push    0x00
    push    _do_overflow
    jmp     exception_handler

; int5 寻址到有效地址以外
global isr5
extern _do_bounds_check
isr5:
    cli
    push    0x00
    push    _do_bounds_check
    jmp     exception_handler

; int6 非法操作码
global isr6
extern _do_invalid_op
isr6:
    cli
    push    0x00
    push    _do_invalid_op
    jmp     exception_handler

; int7 设备故障
global isr7
extern _do_device_fail
isr7:
    cli
    push    _do_device_fail
    jmp     exception_handler

; int8 双故障出错
global isr8
extern _do_double_fault
isr8:
    cli
    push    _do_double_fault
    jmp     exception_handler

; int9 协处理器段错误
global isr9
extern _do_cop_segment
isr9:
    cli
    push    _do_cop_segment
    jmp     exception_handler

; int10 TSS无效
global isr10
extern _do_tss_inval
isr10:
    cli
    push    _do_tss_inval
    jmp     exception_handler

; int11 段不存在
global isr11
extern _do_segment_unpresent
isr11:
    cli
    push    _do_segment_unpresent
    jmp     exception_handler

; int12 堆栈段不存在或者越界
global isr12
extern _do_stack_segment
isr12:
    cli
    push    _do_stack_segment
    jmp     exception_handler

; int13 不符合386保护机制
global isr13
extern _do_general_protection
isr13:
    cli
    push    _do_general_protection
    jmp     exception_handler

; int14 页不在内存
global isr14
extern _do_page_fault
isr14:
    cli
    push    _do_page_fault
    jmp     exception_handler 

; int15 保留
global isr15
extern _do_reserved
isr15:
    cli
    push    0x00
    push    _do_reserved
    jmp     exception_handler

; int16 协处理器出错信号
global isr16
extern _do_cop_error
isr16:
    cli
    push    _do_cop_error
    jmp     exception_handler

; reserved for 17-31

%macro ISR 1
global isr%1
extern _do_reserved
isr%1:
    cli
    push    0x00
    push    _do_reserved
    jmp     exception_handler
%endmacro

ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31


%macro IRQ 1
global irq%1
extern _do_irq%1
irq%1:
    cli
    push    byte    0   ; dummy errcode
    push    _do_irq%1
    jmp     exception_handler
%endmacro

; irq mapped 32 - 47
IRQ 0
;IRQ 1
;IRQ 2
;IRQ 3
;IRQ 4
;IRQ 5
;IRQ 6
;IRQ 7
;IRQ 8
;IRQ 9
;IRQ 10
;IRQ 11
;IRQ 12
;IRQ 13
;IRQ 14
;IRQ 15

extern kernel_data_selector
exception_handler:
    xchg    [esp+4],  eax       ; exchange errcode <-> eax
    xchg    [esp],    ebx       ; exchange function <-> ebx
    
    push    ecx
    push    edx
    push    edi
    push    esi
    push    ebp
    
    push    ds
    push    es
    push    fs
    push    gs

    push    esp                 ; push the [struct isr_regs *]
    push    eax                 ; push errcode
    
    mov     eax,    [kernel_data_selector]
    mov     ds,     ax
    mov     es,     ax
    mov     fs,     ax
    mov     gs,     ax

    call    ebx
    
    pop     eax                 ; skip the esp and eax
    pop     esp
    
    pop     fs
    pop     es
    pop     ds
    pop     gs
    
    pop     ebp
    pop     esi
    pop     edi
    pop     edx
    pop     ecx 

    pop     ebx                ; pop the exchanged function addr
    pop     eax            
    sti
    iret
