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
 *   File:          page.c
 *   Lauguage:      c/cpp
 *   Date:          13-12-20 18:58:40
 *   Descripton:    paging functions
 */

#include <sys/type.h>
#include <arch/i386/page.h>

/* 全局页目录 */
pgdir_t g_pgdir __pgdir;
/* 全局页表 */
pgtbl_t g_pgtbl[1] __pgtbl;

/* 设置页目录宏 */ 
#define set_page_dir(i, pg_tbl_addr, attr) { \
    g_pgdir.item[i] = (pg_t) ((uint32_t) pg_tbl_addr & 0xFFFFF000) | (attr & 0x00000FFF); \
}

/* 设置页表项宏 */
#define set_page_tbl(i, j, phy_addr, attr) { \
    g_pgtbl[i].item[j] = (pg_t) ((uint32_t) phy_addr & 0xFFFFF000) | (attr & 0x00000FFF); \
}

/* 设置整个页表的宏 */
#define fill_page_tbl(idx, phy_addr_start, attr) { \
    int tbli = 0; int start = (phy_addr_start & 0xFFFFF000); \
    while (tbli < PG_TBL_SIZE) { \
        set_page_tbl(idx, tbli, start + (tbli << __PG_OFFSET__), attr); \
        ++tbli; \
    } \
}

/* 开启分页，参数必须是页目录的物理地址 */
#define enable_paging(pgdir_base) { \
    __asm__ __volatile__ ( \
        "movl %0, %%eax\n\t" \
        "movl %%eax, %%cr3\n\t" \
        "movl %%cr0, %%eax\n\t" \
        "orl  $0x80000000, %%eax\n\t" \
        "movl %%eax, %%cr0\n\t" :: "a"(pgdir_base) \
    ); \
}

/* 启动分页 */
void setup_paging(void) {
    memset(&g_pgdir, 0, sizeof(pgdir_t));
    
    set_page_dir(0, logic2phys(&g_pgtbl[0]), 0x3);
    set_page_dir(KERNEL_DIR_SIDX, logic2phys(&g_pgtbl[0]), 0x3);

    fill_page_tbl(0, 0x0, 0x03); 
    /* pg0 0 - 4M */

    enable_paging(logic2phys(&g_pgdir));
}
