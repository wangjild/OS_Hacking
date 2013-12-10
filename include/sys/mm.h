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
 *   File:          protect.h
 *   Lauguage:      c/cpp
 *   Date:          13-12-09 21:32:41
 *   Descripton:    header files for memory manage
 */

#ifndef _SYS_PROTECT_H_
#define _SYS_PROTECT_H_

#include <sys/type.h>

typedef uint32_t pg_t;

extern pg_t pg0[];

#endif
