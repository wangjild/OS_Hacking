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
#include <lib/kstdlib.h>
#include <arch/i386/8259a.h>
#include <arch/i386/io.h>

void set_mask() {

}

static void setIRQ() {

  uint8_t mask1, mask2;
  mask1 = in_byte(PIC1_DATA);
  mask2 = in_byte(PIC2_DATA);

  /* ICW1 */
  out_byte(PIC1_CMD, 0x11); // edge driggered | 8字节中断向量 | 级联 | 需要PIC2
  out_byte(PIC2_CMD, 0x11); 
  io_delay();

  /* ICW2 */
  out_byte(PIC1_DATA, 0x20); // 主PIC2 设置IRQ0-7对应的中断向量号
  out_byte(PIC2_DATA, 0x28);  // 从PIC2 设置IRQ8-15对应的中断向量号
  io_delay();

  /* ICW3 */
  out_byte(PIC1_DATA, 0x04); // IRQ2 对应从8259
  out_byte(PIC2_DATA, 0x02);  // 从8259 对应主IRQ2
  io_delay();

  /* ICW4 */
  out_byte(PIC1_DATA, 0x01); // 主PIC2 x86模式
  out_byte(PIC2_DATA, 0x01);  // 从PIC2
  io_delay();

  /* OCW1 */
  /*所有中断现在全部屏蔽*/
  out_byte(PIC1_DATA, 0xFF);
  out_byte(PIC2_DATA, 0xFF);
  io_delay();
}

void setup_irq() {
  setIRQ(); 
}

void PIC_sendEOI(uint8_t irq) {
  if (irq >= 8)
    out_byte(PIC2_CMD, PIC_EOI);
  out_byte(PIC1_CMD, PIC_EOI);
}
