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
#include "sys/type.h"
#include "sys/protect.h"
#include "sys/irpts.h"

#include "arch/i386/8259a.h"

static void FATAL(const char* str, uint32_t errcode, struct irpt_regs* regs) {
  printk("[FATAL] %s\n", str);
  printk("EIP:    %x:%x\nEFLAGS: %x\nESP:    %x:%x\n",
      regs->cs, regs->eip, regs->eflags, regs->es, regs->uesp);
  printk("ES:     %x\n", regs->es);
  for (;;) ;
}

void _do_divide0_error(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int0: divide 0 error", errcode, regs);
}

void _do_debug(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int1: debug", errcode, regs);
}

void _do_nmi(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int2: nmi", errcode, regs);
}

void _do_debug_break(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int3: break", errcode, regs);
}

void _do_overflow(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int4: overflow", errcode, regs);
}

void _do_bounds_check(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int5: bounds_check", errcode, regs);
}

void _do_invalid_op(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int6: invalid op", errcode, regs);
}

void _do_device_fail(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int7: device fail", errcode, regs);
}

void _do_double_fault(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int8: double fault", errcode, regs);
}

void _do_cop_segment(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int9: coprocesser segment", errcode, regs);
}

void _do_tss_inval(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int10: tss invalid", errcode, regs);
}

void _do_segment_unpresent(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int11: segment unpresent", errcode, regs);
}

void _do_stack_segment(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int12: stack segment unpresent or overflow", errcode, regs);
}

void _do_general_protection(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int13: general protection", errcode, regs);
}

void _do_page_fault(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int14: page fault", errcode, regs);
}

void _do_reserved(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int15: reserved", errcode, regs);
}

void _do_cop_error(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int16: coprocesser error", errcode, regs);
}

void _do_timer(uint32_t errcode, struct irpt_regs* regs) {
  FATAL("int 0x20: timer interrupt", errcode, regs);
  PIC_sendEOI(0);
}


#define IDT_OFFSET 256
struct idt_entry idts[IDT_OFFSET];
struct idt_ptr   idtptr;

#define idt_load() { \
  __asm__ __volatile__ ( \
      "lidt %0" :: "m"(idtptr) \
      ); \
}

void set_idt(uint32_t idx, uint32_t base, uint16_t selector, uint8_t flags) {
  idts[idx].offset_low = base & 0x0FFFF;
  idts[idx].offset_high = (base >> 16) &0x0FFFF;
  idts[idx].selector = selector;
  idts[idx].flags = flags;
}

extern const uint16_t kernel_code_selector;

extern void divide0_error();
extern void debug();
extern void nmi();
extern void debug_break();
extern void overflow();
extern void bounds_check();
extern void invalid_op();
extern void device_fail();
extern void double_fault();
extern void cop_segment();
extern void tss_inval();
extern void segment_unpresent();
extern void stack_segment();
extern void general_protection();
extern void page_fault();
extern void reserved();
extern void cop_error();
extern void exception_handler();

extern void do_timer();

void setup_idt() {
  memset(&idts, 0, sizeof(idts));

  idtptr.base = (uint32_t) &idts;
  idtptr.limit = sizeof(idts) - 1;

  set_idt(0, (uint32_t) &divide0_error, kernel_code_selector, IDT_DPL0 | IDT_TRAP);
  set_idt(1, (uint32_t) &debug, kernel_code_selector, IDT_DPL0 | IDT_TRAP);
  set_idt(2, (uint32_t) &nmi, kernel_code_selector, IDT_DPL0 | IDT_TRAP);
  set_idt(3, (uint32_t) &debug_break, kernel_code_selector, IDT_DPL0 | IDT_TRAP);
  set_idt(13, (uint32_t) &general_protection, kernel_code_selector, IDT_DPL0 | IDT_IRPT);
  
  //设置定时器
  set_idt(0x20, (uint32_t) &_do_timer, kernel_code_selector, IDT_DPL0 | IDT_IRPT);
  set_idt(0x80, (uint32_t) &reserved, kernel_code_selector, IDT_DPL0 | IDT_IRPT);

  idt_load();
}
