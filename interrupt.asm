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

%macro ISR_NOERRCODE 1 
global isr%1
isr%1:
  cli
  push byte 0x00  ;; push dummy error code
  push byte %1
  jmp isr_handler_stub
%endmacro


%macro ISR_ERRCODE 1
global isr%1
isr%1:
  cli
  push byte %1
  jmp isr_handler_stub
%endmacro

%macro IRQ 1
global irq%1
irq%1:
  cli
  push byte %1
  add byte [esp], 32   ;;将IRQ0 映射到 ISR32
  jmp isr_handler_stub
%endmacro

; int0 除0异常 
ISR_NOERRCODE 0

; int1 单步调试
ISR_NOERRCODE 1

; int2 NMI
ISR_NOERRCODE 2

; int3 断点
ISR_NOERRCODE 3
    
; int4 溢出
ISR_NOERRCODE 4

; int5 寻址到有效地址以外
ISR_NOERRCODE 5

; int6 非法操作码
ISR_NOERRCODE 6

; int7 设备故障
ISR_NOERRCODE 7

; int8 双故障出错
ISR_ERRCODE   8

; int9 协处理器段错误
ISR_NOERRCODE 9

; int10 TSS无效
ISR_ERRCODE   10

; int11 段不存在
ISR_ERRCODE   11

; int12 堆栈段不存在或者越界
ISR_ERRCODE   12

; int13 不符合386保护机制
ISR_ERRCODE   13

; int14 页不在内存
ISR_ERRCODE   14

; int15 保留
ISR_NOERRCODE   15

; int16 协处理器出错信号
ISR_NOERRCODE   16

; reserved
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

extern kernel_data_selector
isr_handler_stub:
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

    push    esp
    push    eax                 ; push errcode
    
    mov     eax,    [kernel_data_selector]
    mov     ds,     ax
    mov     es,     ax
    mov     fs,     ax
    
    call    ebx
    
    add     esp,    0x08        ; skip the esp and eax
    
    pop     fs
    pop     es
    pop     ds
    
    pop     ebp
    pop     esi
    pop     edi
    pop     edx
    pop     ecx 
    pop     ebx
    pop     eax
   
    sti
    iret
