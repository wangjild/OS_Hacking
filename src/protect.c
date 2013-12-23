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

#include <sys/protect.h>
#include <arch/i386/page.h>

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
