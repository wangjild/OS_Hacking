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
 *   File:          protect.c
 *   Lauguage:      c/cpp
 *   Date:          13-12-12 20:58:40
 *   Descripton:    Protect Model Operations
 */

#include <protect.h>
#include <sys/page.h>
#include <isr.h>
#include <lib/kstdlib.h>

#define GDT_OFFSET 128
#define SEG_KERNEL_C    SEG_KCODE_FLAT
#define SEG_KERNEL_D    SEG_KDATA_FLAT
const uint16_t gdt_offset = GDT_OFFSET;
const uint16_t kernel_code_selector = SEG_KERNEL_C << 3;
const uint16_t kernel_data_selector = SEG_KERNEL_D << 3;

struct gdt_entry    gdts[GDT_OFFSET];
struct gdt_ptr      gdtptr  = {0, GDT_OFFSET};

#define set_gdt(idx, base, limit, attr) \
{ \
    gdts[idx].base_low = base & 0x0FFFF; \
    gdts[idx].base_mid = (base >> 16) & 0x0FF; \
    gdts[idx].base_high = (base >> 24) & 0x0FF; \
    gdts[idx].limit_low = limit & 0x0FFFF; \
    gdts[idx].attr_limit_high = ((limit >> 8) & 0x0F00) | attr; \
}

extern void gdt_flush();

void setup_gdt() {
    memset(&gdts, 0, sizeof(struct gdt_entry) * GDT_OFFSET);

    gdtptr.base = (uint32_t) &gdts;
    gdtptr.limit = sizeof(gdts) - 1;

    set_gdt(SEG_DUMMY, 0, 0, 0); // DUMMY GDT
    set_gdt(SEG_KERNEL_C, 0, 0xFFFFFF, SEG_32 | SEG_4K | SEG_CAR); // KERNEL CODE GDT
    set_gdt(SEG_KERNEL_D, 0, 0xFFFFFF, SEG_32 | SEG_4K | SEG_DRW); // KERNEL DATA GDT
    gdt_flush();
}


#define IDT_OFFSET 256
struct idt_entry idts[IDT_OFFSET];
struct idt_ptr   idtptr;

#define idt_flush() { \
    __asm__ __volatile__ ( \
            "lidt %0" :: "m"(idtptr) \
            ); \
}

void set_idt_gate(uint32_t idx, uint32_t base, uint16_t selector, uint8_t flags) {
    idts[idx].offset_low = base & 0x0FFFF;
    idts[idx].offset_high = (base >> 16) &0x0FFFF;
    idts[idx].selector = selector;
    idts[idx].flags = flags;
}

#define set_isr_gate(num) \
    set_idt_gate(num, (uint32_t) &isr##num, kernel_code_selector, IDT_DPL0 | IDT_TRAP)

#define set_irq_gage(num, handler) \
    set_idt_gate(num+32, handler, kernel_code_selector, IDT_DPL0 | IDT_TRAP)


void setup_idt() {
    memset(&idts, 0, sizeof(idts));

    idtptr.base = (uint32_t) &idts;
    idtptr.limit = sizeof(idts) - 1;

    set_isr_gate(0);
    set_isr_gate(1);
    set_isr_gate(2);
    set_isr_gate(3);
    set_isr_gate(4);
    set_isr_gate(5);
    set_isr_gate(6);
    set_isr_gate(7);
    set_isr_gate(8);
    set_isr_gate(9);
    set_isr_gate(10);
    set_isr_gate(11);
    set_isr_gate(12);
    set_isr_gate(13);
    set_isr_gate(14);
    set_isr_gate(15);
    set_isr_gate(16);
    set_isr_gate(17);
    set_isr_gate(18);
    set_isr_gate(19);
    set_isr_gate(20);
    set_isr_gate(21);
    set_isr_gate(22);
    set_isr_gate(23);
    set_isr_gate(24);
    set_isr_gate(25);
    set_isr_gate(26);
    set_isr_gate(27);
    set_isr_gate(28);
    set_isr_gate(29);
    set_isr_gate(30);
    set_isr_gate(31);

    idt_flush();
}
