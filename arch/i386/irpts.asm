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
global divide0_error
extern _do_divide0_error
divide0_error:
    push    0x00 ; 当做错误码 push 入栈
    push    _do_divide0_error
    jmp     exception_handler

; int1 单步调试
global debug
extern _do_debug
debug:
    push    0x00
    push    _do_debug
    jmp     exception_handler

; int2 NMI
global nmi
extern _do_nmi
nmi:
    push    0x00
    push    _do_nmi
    jmp     exception_handler

; int3 断点
global debug_break
extern _do_debug_break
debug_break:
    push    0x00
    push    _do_debug_break
    jmp     exception_handler
    
; int4 溢出
global overflow
extern _do_overflow
overflow:
    push    0x00
    push    _do_overflow
    jmp     exception_handler

; int5 寻址到有效地址以外
global bounds_check
extern _do_bounds_check
bounds_check:
    push    0x00
    push    _do_bounds_check
    jmp     exception_handler

; int6 非法操作码
global invalid_op
extern _do_invalid_op
invalid_op:
    push    0x00
    push    _do_invalid_op
    jmp     exception_handler

; int7 设备故障
global device_fail
extern _do_device_fail
device_fail:
    push    _do_device_fail
    jmp     exception_handler

; int8 双故障出错
global double_fault
extern _do_double_fault
double_fault:
    push    _do_double_fault
    jmp     exception_handler

; int9 协处理器段错误
global cop_segment
extern _do_cop_segment
cop_segment:
    push    _do_cop_segment
    jmp     exception_handler

; int10 TSS无效
global tss_inval
extern _do_tss_inval
tss_inval:
    push    _do_tss_inval
    jmp     exception_handler

; int11 段不存在
global segment_unpresent
extern _do_segment_unpresent
segment_unpresent:
    push    _do_segment_unpresent
    jmp     exception_handler

; int12 堆栈段不存在或者越界
global stack_segment
extern _do_stack_segment
stack_segment:
    push    _do_stack_segment
    jmp     exception_handler

; int13 不符合386保护机制
global general_protection
extern _do_general_protection
general_protection:
    push    _do_general_protection
    jmp     exception_handler

; int14 页不在内存
global page_fault
extern _do_page_fault
page_fault:
    push _do_page_fault
    jmp     exception_handler 

; int15 保留
global reserved
extern _do_reserved
reserved:
    push    0x00
    push    _do_reserved
    jmp     exception_handler

; int16 协处理器出错信号
global cop_error
extern _do_cop_error
cop_error:
    push    _do_cop_error
    jmp     exception_handler

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
    push    esp
    push    eax                 ; push errcode
    mov     eax,    [kernel_data_selector]
    mov     ds,     ax
    mov     es,     ax
    mov     fs,     ax
    call    ebx
    add     esp,    0x08
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
    iret
