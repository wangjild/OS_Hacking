/* kernel.c - the C part of the kernel */
/* Copyright (C) 1999  Free Software Foundation, Inc.
     
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
     
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
*/

/* 
 * @brief Catos kernel entry
 * @author Wang Jing (wangjild@gmail.com)
*/
     
#include <multiboot.h>
#include "lib/kstdio.h"
#include "lib/kstdlib.h"

//#include <sys/mm.h>
#include <isr.h>
#include <sys/page.h>
#include <sys/8259a.h>
#include <sys/timer.h>
#include <mm/memory.h>

/* Macros. */
     
/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

/* Forward declarations. */
void kernel_main (unsigned long addr, unsigned long magic);

/* Check if MAGIC is valid and print the Multiboot information structure
pointed by ADDR. */
void
kernel_main (unsigned long addr, unsigned long magic)
{

    setup_paging();
    setup_gdt();

    cls ();
    
    printk("Setup IDT...\n");
    setup_idt();
    printk("Done\n");
    
    printk("Setup IRQ...\n");
    setup_irq();
    printk("Done\n");

    multiboot_info_t *mbi;
     
    /* Am I booted by a Multiboot-compliant boot loader? */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        printk ("Invalid magic number: 0x%x\n", (unsigned) magic);
        return;
    }
     
    /* Set MBI to the address of the Multiboot information structure. */
    /* We already in kernel 32 mode, let's fix the vaddr */
    mbi = (multiboot_info_t *) (addr + KERNEL_OFFSET);
  
    /* Are mmap_* valid? */
    if (CHECK_FLAG (mbi->flags, 6))
    {
        show_boot_mem(mbi);
        show_kernel_image_mem();
    } else {
        // panic
    }


    
    sti();
   
    /*
    printk("Setup TIMER...\n");
     setup_timer(10);
    printk("Done\n");
    */

    printk("Hello CatOS!\n");

    __asm__ __volatile__ ("hlt\r\n");
}
