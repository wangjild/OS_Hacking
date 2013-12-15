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

static void FATAL(const char* str, struct irpt_regs* regs) {
    printk("EIP:    %x:%x\nEFLAGS:    %x\nESP:    %x:%x\n",
        regs->cs, regs->eip, regs->eflags, regs->es, regs->uesp);
    for (;;) ;
}

void _do_divide0_error(struct irpt_regs* regs) {
    FATAL("int0: divide 0 error", regs);
}

void _do_debug(struct irpt_regs* regs) {
    FATAL("int1: debug", regs);
}

void _do_nmi(struct irpt_regs* regs) {
    FATAL("int2: nmi", regs);
}

void _do_debug_break(struct irpt_regs* regs) {
    FATAL("int3: break", regs);
}

void _do_overflow(struct irpt_regs* regs) {
    FATAL("int4: overflow", regs);
}

void _do_bounds_check(struct irpt_regs* regs) {
    FATAL("int5: bounds_check", regs);
}

void _do_invalid_op(struct irpt_regs* regs) {
    FATAL("int6: invalid op", regs);
}

void _do_device_fail(struct irpt_regs* regs) {
    FATAL("int7: device fail", regs);
}

void _do_double_fault(struct irpt_regs* regs) {
    FATAL("int8: double fault", regs);
}

void _do_cop_segment(struct irpt_regs* regs) {
    FATAL("int9: coprocesser segment", regs);
}

void _do_tss_inval(struct irpt_regs* regs) {
    FATAL("int10: tss invalid", regs);
}

void _do_segment_unpresent(struct irpt_regs* regs) {
    FATAL("int11: segment unpresent", regs);
}

void _do_stack_segment(struct irpt_regs* regs) {
    FATAL("int12: stack segment unpresent or overflow", regs);
}

void _do_general_protection(struct irpt_regs* regs) {
    FATAL("int13: general protection", regs);
}

void _do_page_fault(struct irpt_regs* regs) {
    FATAL("int14: page fault", regs);
}

void _do_reserved(struct irpt_regs* regs) {
    FATAL("int15: reserved", regs);
}

void _do_cop_error(struct irpt_regs* regs) {
    FATAL("int16: coprocesser error", regs);
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
    set_idt(13, (uint32_t) &general_protection, kernel_code_selector, IDT_DPL0 | IDT_IRPT);
    
    idt_load();
}
