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
#include <lib/debug.h>
#include <protect.h>
#include <isr.h>
#include <sys/io.h>

#include <sys/8259a.h>

void _do_divide0_error(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int0: divide 0 error", errcode, regs);
}

void _do_debug(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int1: debug", errcode, regs);
}

void _do_nmi(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int2: nmi", errcode, regs);
}

void _do_debug_break(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int3: break", errcode, regs);
}

void _do_overflow(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int4: overflow", errcode, regs);
}

void _do_bounds_check(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int5: bounds_check", errcode, regs);
}

void _do_invalid_op(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int6: invalid op", errcode, regs);
}

void _do_device_fail(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int7: device fail", errcode, regs);
}

void _do_double_fault(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int8: double fault", errcode, regs);
}

void _do_cop_segment(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int9: coprocesser segment", errcode, regs);
}

void _do_tss_inval(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int10: tss invalid", errcode, regs);
}

void _do_segment_unpresent(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int11: segment unpresent", errcode, regs);
}

void _do_stack_segment(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int12: stack segment unpresent or overflow", errcode, regs);
}

void _do_general_protection(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int13: general protection", errcode, regs);
}

void _do_page_fault(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int14: page fault", errcode, regs);
}

void _do_reserved(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int15: reserved", errcode, regs);
}

void _do_cop_error(uint32_t errcode, struct isr_regs* regs) {
    FATAL("int16: coprocesser error", errcode, regs);
}

static void setup_pic() {

  /* ICW1 */
  out_byte(PIC1_CMD, 0x11); // edge driggered | 8字节中断向量 | 级联 | 需要PIC2
  io_delay();
  out_byte(PIC2_CMD, 0x11); 
  io_delay();

  /* ICW2 */
  out_byte(PIC1_DATA, 0x20); // 主PIC2 设置IRQ0-7对应的中断向量号
  io_delay();
  out_byte(PIC2_DATA, 0x28);  // 从PIC2 设置IRQ8-15对应的中断向量号
  io_delay();

  /* ICW3 */
  out_byte(PIC1_DATA, 0x04); // IRQ2 对应从8259
  io_delay();
  out_byte(PIC2_DATA, 0x02);  // 从8259 对应主IRQ2
  io_delay();

  /* ICW4 */
  out_byte(PIC1_DATA, 0x01); // 主PIC2 x86模式
  io_delay();
  out_byte(PIC2_DATA, 0x01);  // 从PIC2
  io_delay();

  /* OCW1 */
  /*所有中断现在全部屏蔽*/
  out_byte(PIC1_DATA, 0xFF);
  io_delay();
  out_byte(PIC2_DATA, 0xFF);
  io_delay();
}

void setup_irq() {
  setup_pic(); 
}

void enable_irq(uint8_t num) {
    uint8_t oldmask, newmask;

    if (num < 8) {
        oldmask = in_byte(PIC1_DATA);
        newmask = oldmask & ~(1 << num);
        if (newmask != oldmask) {
            out_byte(PIC1_DATA, newmask);
        }
    } else if (num < 16){
        oldmask = in_byte(PIC2_DATA);
        newmask = oldmask & ~(1 << (num - 8));
        if (newmask != oldmask) {
            out_byte(PIC2_DATA, newmask);
        }
    }

}

void PIC_sendEOI(uint8_t irq) {
  if (irq >= 8) {
    out_byte(PIC2_CMD, PIC_EOI);
    nop();
  }

  out_byte(PIC1_CMD, PIC_EOI);
  nop();
}
