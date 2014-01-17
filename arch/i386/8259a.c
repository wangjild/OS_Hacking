/*
 *  Copyright (C) 2014  Wang Jing <wangjild@gmail.com>
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
 *   File:          arch/i386/page.h
 *   Lauguage:      c/cpp
 *   Date:          14-01-13 18:29:47
 *   Descripton:    Intel 8259
 */

static void setIRQ(int isqvec) {
    int slave_isqvec = isqvec + 8;
    __asm__ __volatile__(
        "movb   $0x11,  %%al\n\t" // edge driggered | 8字节中断向量 | 级联 | 需要ICW4
        "outb   %%al,   $0x20\n\t" // 主ICW1
        "nop\n\tnop\n\tnop\n\t"
        
        "outb   %%al,   $0xA0\n\t" // 从ICW1
        "nop\n\tnop\n\tnop\n\t"
        
        "movb   %0,     %%al\n\t" // 主ICW2 设置IRQ0-7对应的中断向量号
        "outb   %%al,   $0x21\n\t"
        "nop\n\tnop\n\tnop\n\t"
       
        "movb   %1,     %%al\n\t" // 从ICW2 设置IRQ8-15对应的中断向量号
        "outb   %%al,   $0xA1\n\t" 
        "nop\n\tnop\n\tnop\n\t"

        "movb   $0x04,  %%al\n\t" // IRQ2 对应从8259
        "outb   %%al,   $0x21\n\t"
        "nop\n\tnop\n\tnop\n\t"


        "movb   $0x02,  %%al\n\t" // 从8259 对应主IRQ2
        "outb   %%al,   $0xA1\n\t"
        "nop\n\tnop\n\tnop\n\t"

        "movb   $0x01,  %%al\n\t" // 主ICW4 x86模式 
        "outb   %%al,   $0x21\n\t"
        "nop\n\tnop\n\tnop\n\t"

        "outb   %%al,   $0xA1\n\t" // 从ICW4
        "nop\n\tnop\n\tnop\n\t" 
        :: "m" (isqvec), "m"(slave_isqvec)
    );

    __asm__ __volatile__ (
        "movb   $0xFF,  %%al\n\t"
        "outb   %%al,   $0x21\n\t"
        "nop\n\tnop\n\tnop\n\t" 
        
        "movb   $0xFF,  %%al\n\t"
        "outb   %%al,   $0xA1\n\t"
        "nop\n\tnop\n\tnop\n\t" :: 
    );

}

void setup_irq() {
    setIRQ(0x20); 
}
