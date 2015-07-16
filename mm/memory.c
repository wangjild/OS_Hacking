#include <boot/multiboot.h>
#include <lib/kstdio.h>
#include <sys/type.h>

void show_boot_mem(multiboot_info_t *mbi) {
    multiboot_memory_map_t * mmap;

    printk ("mmap_addr = 0x%x, mmap_length = 0x%x\n",
            (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
    for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
            (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
            mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                + mmap->size + sizeof(mmap->size))) {
        printk ("    size = 0x%x, base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n",
                (unsigned) mmap->size,
                (unsigned) (mmap->addr >> 32),
                (unsigned) (mmap->addr & 0x0ffffffff),
                (unsigned) (mmap->len >> 32),
                (unsigned) (mmap->len & 0x0ffffffff),
                (unsigned) mmap->type);
    }
}

extern uint32_t _kernel_image_start;
extern uint32_t _kernel_image_end;

void show_kernel_image_mem() {
    printk("kernel in memory start: 0x%x\n", &_kernel_image_start);
    printk("kernel in memory end:   0x%x\n", &_kernel_image_end);
    printk("kernel in memory used:   %d KB\n\n", (&_kernel_image_end - &_kernel_image_start + 1023) / 1024);
}
