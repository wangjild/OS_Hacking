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
 *   File:          irpts.c
 *   Lauguage:      c/cpp
 *   Date:          13-12-14 22:58:40
 *   Descripton:    interupts 0 - 32 handlers and installers
 */
#include <sys/type.h>
#include <sys/protect.h>
#include <sys/irpts.h>

static void FATAL(const char* str, uint32_t* esp, uint32_t errcode) {
    printk("EIP:\t%04x:%x\nEFLAGS:\t%04x\nESP:\t%04x:%x\n",
        esp[1], esp[0], esp[2], esp[4], esp[3]);
    for (;;) ;
}

void _do_divide0_error(uint32_t esp, uint32_t errcode) {
    FATAL("divide 0 error", (uint32_t*) esp, errcode);
}

void _do_general_protection(uint32_t esp, uint32_t errcode) {
    FATAL("general protection", (uint32_t*) esp, errcode);
}

#define IDT_OFFSET 256
struct idt_entry idts[IDT_OFFSET];
struct idt_ptr   idtptr;

#define idt_load() { \
    __asm__ __volatile__ ( \
        "lidt %0" :: "m"(idtptr) \
    ); \
}

static void set_idt(uint32_t idx, uint32_t base, uint16_t selector, uint8_t flags) {
    idts[idx].offset_low = base & 0x0FFFF;
    idts[idx].offset_high = (base >> 16) &0x0FFFF;
    idts[idx].selector = selector;
    idts[idx].flags = flags;
}

extern const uint16_t kernel_code_selector;
void divide0_error();
void general_protection();

void setup_idt() {
    memset(&idts, 0, sizeof(idts));
    
    idtptr.base = (uint32_t) &idts;
    idtptr.limit = sizeof(idts) - 1;
    
    set_idt(0, (uint32_t) &divide0_error, kernel_code_selector, IDT_DPL0 | IDT_TRAP);
    
    idt_load();
    //set_idt(13, (uint32_t) &general_protection, kernel_code_selector, IDT_DPL0 | IDT_IRPT);
}
