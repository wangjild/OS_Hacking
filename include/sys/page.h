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
 *   File:          sys/page.h
 *   Lauguage:      c/cpp
 *   Date:          13-12-02 21:56:23
 *   Descripton:    page structure 
 */


#ifndef _SYS_PAGE_H_
#define _SYS_PAGE_H_

#include "sys/type.h"

typedef uint32_t pg_t;

#define PG_DIR_SIZE (1 << 10)
#define PG_TBL_SIZE PG_DIR_SIZE

#define __PG_OFFSET__   12

typedef struct pgdir {
    pg_t item[PG_DIR_SIZE];
}pgdir_t;

typedef pgdir_t pgtbl_t;
extern pgdir_t  g_pgdir __pgdir;
extern pgtbl_t  g_pgtbl[1] __pgtbl; // max 4G

#define logic2phys(viraddr) ((void*) viraddr) + 0x0 - KERNEL_OFFSET

#define KERNEL_DIR_SIDX   (KERNEL_OFFSET >> 22)
#define KERNEL_DIR_EIDX   PG_DIR_SIZE

void setup_page (void) ;

#endif
