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
;;   Descripton:    interrupt entries

[BITS 32]

;除0异常 
global _divide0_error

extern _do_divide0_error
_divide0_error:
    push    0x00 ; 当做错误码 push 入栈
    push    _do_divide0_error
    jmp     exception_handler


extern kernel_data_selector
exception_handler:
    xchg    [esp+4],  eax
    xchg    esp,    ebx
    push    ecx
    push    edx
    push    edi
    push    esi
    push    ebp
    push    ds
    push    es
    push    fs
    push    eax
    lea     eax,    [esp+44]
    push    eax
    mov     eax,    [kernel_data_selector]
    mov     ds,     ax
    mov     es,     ax
    mov     fs,     ax
    call    [ebx]
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
