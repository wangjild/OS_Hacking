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
#include "arch/i386/page.h"
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

#define set_isr_gate(num, handler) \
    set_idt_gate(num, handler, kernel_code_selector, IDT_DPL0 | IDT_TRAP)

#define set_irq_gage(num, handler) \
    set_idt_gate(num+32, handler, kernel_code_selector, IDT_DPL0 | IDT_TRAP)


void setup_idt() {
    memset(&idts, 0, sizeof(idts));

    idtptr.base = (uint32_t) &idts;
    idtptr.limit = sizeof(idts) - 1;

    set_isr_gate(0, (uint32_t) &_do_divide0_error);
    set_isr_gate(1, (uint32_t) &_do_debug);
    set_isr_gate(2, (uint32_t) &_do_nmi);
    set_isr_gate(3, (uint32_t) &_do_debug_break);
    set_isr_gate(4, (uint32_t) &_do_overflow);
    set_isr_gate(5, (uint32_t) &_do_bounds_check);
    set_isr_gate(6, (uint32_t) &_do_invalid_op);
    set_isr_gate(7, (uint32_t) &_do_device_fail);
    set_isr_gate(8, (uint32_t) &_do_double_fault);
    set_isr_gate(9, (uint32_t) &_do_cop_segment);
    set_isr_gate(10, (uint32_t) &_do_tss_inval);
    set_isr_gate(11, (uint32_t) &_do_segment_unpresent);
    set_isr_gate(12, (uint32_t) &_do_stack_segment);
    set_isr_gate(13, (uint32_t) &_do_general_protection);
    set_isr_gate(14, (uint32_t) &_do_page_fault);
    set_isr_gate(15, (uint32_t) &_do_reserved);
    set_isr_gate(16, (uint32_t) &_do_cop_error);
   
    for (int i = 17; i < 32; ++i) {
        set_isr_gate(i, (uint32_t) &_do_reserved);
    }

    idt_flush();
}
