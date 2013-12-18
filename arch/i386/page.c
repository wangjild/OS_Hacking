#include <sys/type.h>
#include <arch/i386/page.h>

pgdir_t g_pgdir __pgdir;
pgtbl_t g_pgtbl[2] __pgtbl;

#define set_page_dir(i, pg_tbl_addr, attr) { \
    *((pg_t*) vir2phys(&g_pgdir.item[i])) = (vir2phys(pg_tbl_addr) & 0xFFFFF000) | (attr & 0x00000FFF); \
}

#define set_page_tbl(i, j, phy_addr, attr) { \
    *((pg_t*) vir2phys(&g_pgtbl[i].item[j])) = (phy_addr  & 0xFFFFF000) | (attr & 0x00000FFF); \
}

#define fill_page_tbl(idx, phy_addr_start, attr) { \
    int i = 0; int start = (phy_addr_start & 0xFFFFF000); \
    while (i < PG_TBL_SIZE) { \
        set_page_tbl(idx, i, start + (i << __PG_OFFSET__), attr); \
        ++i; \
    } \
}

#define PHY_ADDR_START 0x00

#define enable_paging() { \
    __asm__ __volatile__ ( \
        "movl %0, %%eax\n\t" \
        "movl %%eax, %%cr3\n\t" \
        "movl %%cr0, %%eax\n\t" \
        "orl  $0x80000000, %%eax\n\t" \
        "movl %%eax, %%cr0\n\t" :: "a"(vir2phys(&g_pgdir)) \
    ); \
}

void setup_paging(void) {
    int idx = 0;
    int i = KERNEL_DIR_SIDX;
    for ( ; i < KERNEL_DIR_EIDX && idx < 2; i++, idx++) {
        set_page_dir(i, &(g_pgtbl[idx]), 0x7);
    }

    i = 0;
    for (; i < 4; ++i) {
        fill_page_tbl(i, PHY_ADDR_START + i << (__PG_OFFSET__ + 10), 0x07); /* pg0 0K - 4K */
    }

    enable_paging();
}
