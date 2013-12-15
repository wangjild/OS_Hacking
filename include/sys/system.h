/*
 *  Copyright (C) 2013  Wang Jing <wangjild@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *   Author:        wangjild <wangjild@gmail.com>
 *   Github:        https://github.com/wangjild/OS_Hacking
 *   File:          system.h
 *   Lauguage:      c/cpp
 *   Date:          13-12-15 22:12:35
 *   Descripton:    
 */

#include <sys/types.h>
struct irpt_regs
{
    uint32_t    err_code;  /* push */
    uint32_t    fs, es, ds; /* 压入上一个段。 */
    uint32_t    ebp, esi, edi, edx, ecx; /* 由push 压入*/
    uint32_t    ebx, eax;   /* 由exchange 压入*/
    uint32_t    eip, cs, eflags, uesp, ss; /* 由处理器自动压入。 */ 
};
