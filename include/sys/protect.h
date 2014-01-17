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
 *   Date:          13-12-02 21:53:03
 *   Descripton:    header files for the GDT, LDT, TSS 
 */

#ifndef _SYS_PROTECT_H_
#define _SYS_PROTECT_H_
#include <sys/type.h>

/* 
 *  @brief Data/Code Segment Descriptor 
 *  31              24              16              8             0
 *  7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0
 *  |--------------------------------------------------------------|
 *  |              | |D| |A|       | | D | |       |               |
 *  | BASE_HIGN:8  |G|/| |V| Lmt:4 |P| P |S|  TYPE |  BASE_MID:8   |     
 *  |              | |B| |L|       | | L | |       |               |
 *  |---------------------------------------------------------------
 *  7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0
 *  |--------------------------------------------------------------|
 *  |        BASE_LOW:16           |         LIMIT_LOW:16          |
 *  |--------------------------------------------------------------|
 *
 ***** 以下说明Copy自于渊的《Orange's 一个操作系统的实现》随书源码  
 *  (1) P:   存在(Present)位。
 *                P=1 表示描述符对地址转换是有效的，或者说该描述符所描述的段存在，即在内存中；
 *                P=0 表示描述符对地址转换无效，即该段不存在。使用该描述符进行内存访问时会引起异常。
 *
 *  (2) DPL: 表示描述符特权级(Descriptor Privilege level)，共2位。它规定了所描述段的特权级，用于特权检查，以决定对该段能否访问。 
 *
 * (3) S:    说明描述符的类型。
 *                对于存储段描述符而言，S=1，以区别与系统段描述符和门描述符(S=0)。 
 *
 * (4) TYPE: 说明存储段描述符所描述的存储段的具体属性。
 *
 *      数据段类型        类型值                说明
 *                        ----------------------------------
 *                        0                只读 
 *                        1                只读、已访问 
 *                        2                读/写 
 *                        3                读/写、已访问 
 *                        4                只读、向下扩展 
 *                        5                只读、向下扩展、已访问 
 *                        6                读/写、向下扩展 
 *                        7                读/写、向下扩展、已访问 
 *                
 *                        类型值                说明
 *      代码段类型        ----------------------------------
 *                        8                只执行 
 *                        9                只执行、已访问 
 *                        A                执行/读 
 *                        B                执行/读、已访问 
 *                        C                只执行、一致码段 
 *                        D                只执行、一致码段、已访问 
 *                        E                执行/读、一致码段 
 *                        F                执行/读、一致码段、已访问 
 *                
 *      系统段类型        类型编码        说明
 *                        ----------------------------------
 *                        0                <未定义>
 *                        1                可用286TSS
 *                        2                LDT
 *                        3                忙的286TSS
 *                        4                286调用门
 *                        5                任务门
 *                        6                286中断门
 *                        7                286陷阱门
 *                        8                未定义
 *                        9                可用386TSS
 *                        A                <未定义>
 *                        B                忙的386TSS
 *                        C                386调用门
 *                        D                <未定义>
 *                        E                386中断门
 *                        F                386陷阱门
 *
 * (5) G:    段界限粒度(Granularity)位。
 *                G=0 表示界限粒度为字节；
 *                G=1 表示界限粒度为4K 字节。
 *           注意，界限粒度只对段界限有效，对段基地址无效，段基地址总是以字节为单位。 
 *
 * (6) D:    D位是一个很特殊的位，在描述可执行段、向下扩展数据段或由SS寄存器寻址的段(通常是堆栈段)的三种描述符中的意义各不相同。 
 *           ⑴ 在描述可执行段的描述符中，D位决定了指令使用的地址及操作数所默认的大小。
 *                ① D=1表示默认情况下指令使用32位地址及32位或8位操作数，这样的代码段也称为32位代码段；
 *                ② D=0 表示默认情况下，使用16位地址及16位或8位操作数，这样的代码段也称为16位代码段，它与80286兼容。可以使用地址大小前缀和操作数大小前缀分别改变默认的地址或操作数的大小。 
 *           ⑵ 在向下扩展数据段的描述符中，D位决定段的上部边界。
 *                ① D=1表示段的上部界限为4G；
 *                ② D=0表示段的上部界限为64K，这是为了与80286兼容。 
 *           ⑶ 在描述由SS寄存器寻址的段描述符中，D位决定隐式的堆栈访问指令(如PUSH和POP指令)使用何种堆栈指针寄存器。
 *                ① D=1表示使用32位堆栈指针寄存器ESP；
 *                ② D=0表示使用16位堆栈指针寄存器SP，这与80286兼容。 
 *
 * (7) AVL:  软件可利用位。80386对该位的使用未左规定，Intel公司也保证今后开发生产的处理器只要与80386兼容，就不会对该位的使用做任何定义或规定
 * */

struct gdt_entry {
    uint16_t limit_low;     /* LIMIT 0-15   */
    uint16_t base_low;      /* BASE  0-15   */
    uint8_t base_mid;       /* BASE  16-23  */
    uint16_t attr_limit_high;/* LIMIT 16-19 AVL:1 Resvd:1 D/B:1 G:1 TYPE:4 S:1 DPL:2 P:1 */
    uint8_t base_high;      /* BASE  24-32  */
} __attribute__ ((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed));

// G flag
#define SEG_4K 0x8000
// D flag
#define SEG_32 0x4000

// DPL flag
#define SEG_DPL0 0x00
#define SEG_DPL1 0x20
#define SEG_DPL2 0x40
#define SEG_DPL3 0x60

// DATA TYPE (S:1 TYPE:4) 
#define SEG_DR      0x90
#define SEG_DRW     0x92
#define SEG_DRWA    0x93

// CODE TYPE (S:1 TYPE:4)
#define SEG_C       0x98
#define SEG_CA      0x99
#define SEG_CR      0x9A
#define SEG_CAR     0x9B
#define SEG_CC      0x9C
#define SEG_CAC     0x9D
#define SEG_CRC     0x9E
#define SEG_CARC    0x9F

// SYSTEM TYPE (S:0 TYPE:4)
#define SEG_LDT     0x82
#define SEG_TG      0x85
#define SEG_TSS     0x89
#define SEG_CGATE   0x8C
#define SEG_IGATE   0x8E
#define SEG_TGATE   0x8F


#define SEG_DUMMY    0
#define SEG_KCODE_FLAT   1
#define SEG_KDATA_FLAT   2

struct gate_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  dcount;
    uint8_t  attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  resved;
    uint8_t  flags;
    uint16_t offset_high;
} __attribute__ ((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed));

#define IDT_DPL0    0x80
#define IDT_DPL1    0xA0
#define IDT_DPL2    0xC0
#define IDT_DPL3    0xE0
#define IDT_IRPT    0x0E
#define IDT_TRAP    0x0F

extern const uint16_t kernel_code_selector;
extern const uint16_t kernel_data_selector;

#endif
