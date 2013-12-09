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
#include <stdio.h>
#include <stdlib.h>

#include <sys/i8259.h>
#include <sys/mm.h>
     
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
    multiboot_info_t *mbi;
     
    /* Clear the screen. */
    cls ();
     
    /* Am I booted by a Multiboot-compliant boot loader? */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
        return;
    }
     
    /* Set MBI to the address of the Multiboot information structure. */
    mbi = (multiboot_info_t *) addr;
  
    /* Print out the flags. */
    printf ("flags = 0x%x\n", (unsigned) mbi->flags);
     
    /* Are mem_* valid? */
    if (CHECK_FLAG (mbi->flags, 0))
        printf ("mem_lower = %uKB, mem_upper = %uKB\n",
            (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);
     
    /* Is boot_device valid? */
    if (CHECK_FLAG (mbi->flags, 1))
        printf ("boot_device = 0x%x\n", (unsigned) mbi->boot_device);
     
    /* Is the command line passed? */
    if (CHECK_FLAG (mbi->flags, 2))
        printf ("cmdline = %s\n", (char *) mbi->cmdline);
     
    /* Are mods_* valid? */
    if (CHECK_FLAG (mbi->flags, 3))
    {
        multiboot_module_t *mod;
        int i;
     
        printf ("mods_count = %d, mods_addr = 0x%x\n",
            (int) mbi->mods_count, (int) mbi->mods_addr);
        for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
            i < mbi->mods_count;
            i++, mod++)
            printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                (unsigned) mod->mod_start,
                (unsigned) mod->mod_end,
                (char *) mod->cmdline);
    }
     
    /* Bits 4 and 5 are mutually exclusive! */
    if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
    {
        printf ("Both bits 4 and 5 are set.\n");
        return;
    }
     
    /* Is the symbol table of a.out valid? */
        if (CHECK_FLAG (mbi->flags, 4))
    {
        multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);
     
        printf ("multiboot_aout_symbol_table: tabsize = 0x%0x, "
                   "strsize = 0x%x, addr = 0x%x\n",
                   (unsigned) multiboot_aout_sym->tabsize,
                   (unsigned) multiboot_aout_sym->strsize,
                   (unsigned) multiboot_aout_sym->addr);
    }
     
    /* Is the section header table of ELF valid? */
    if (CHECK_FLAG (mbi->flags, 5))
    {
        multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);
     
        printf ("multiboot_elf_sec: num = %u, size = 0x%x,"
                   " addr = 0x%x, shndx = 0x%x\n",
                   (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
                   (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
    }
     
    /* Are mmap_* valid? */
    if (CHECK_FLAG (mbi->flags, 6))
    {
        multiboot_memory_map_t *mmap;
     
        printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n",
            (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
        for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
            (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
            mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                + mmap->size + sizeof(mmap->size)))
            printf ("    size = 0x%x, base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n",
                (unsigned) mmap->size,
                (unsigned) (mmap->addr >> 32),
                (unsigned) (mmap->addr & 0x0ffffffff),
                (unsigned) (mmap->len >> 32),
                (unsigned) (mmap->len & 0x0ffffffff),
                (unsigned) mmap->type);
    }

    printf("Hello CatOS!\n");
    printf("pg address:%x\n", &pg0[0]);
    printf("pg0 %d\n", pg0[0]);
}
