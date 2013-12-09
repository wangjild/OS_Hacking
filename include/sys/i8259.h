/*
 *  Copyright (C) 2013  Wang Jing
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
 *   File:          i8259.h
 *   Lauguage:      c/cpp
 *   Date:          13-12-09 21:56:36
 *   Descripton:    interrupt functions
 */

#ifndef _SYS_I8259_H_
#define _SYS_I8259_H_

#include <sys/type.h>

void
out_byte(uint16_t port, uint8_t val) {
    asm volatile("out %%al, %%dx\n\tnop\n\tnop" : : "a"(val), "d"(port) );
}

uint8_t 
in_byte (uint16_t port) {
    uint8_t result;
    asm volatile("in %%dx, %%al\n\tnop\n\tnop\n\t" : "=a"(result) : "d"(port));
    return result;
}

#endif
