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
 *   File:          arch/i386/page.h
 *   Lauguage:      c/cpp
 *   Date:          14-01-13 18:26:03
 *   Descripton:    Intel 8259
 */

#ifndef _8259A_H_
#define _8259A_H_

#define PIC1_CMD  0x20
#define PIC1_DATA  (PIC1_CMD + 1)

#define PIC2_CMD  0xA0
#define PIC2_DATA  (PIC2_CMD + 1)

void setup_irq();

#define PIC_EOI 0x20

void PIC_sendEOI(uint8_t irq) {
  if (irq >= 8)
    out_byte(PIC2_CMD, PIC_EOI);
  out_byte(PIC1_CMD, PIC_EOI);
}
#endif
