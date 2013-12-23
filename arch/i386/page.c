#include <sys/type.h>
#include <arch/i386/page.h>

pgdir_t g_pgdir __pgdir;
pgtbl_t g_pgtbl[1] __pgtbl;

#define set_page_dir(i, pg_tbl_addr, attr) { \
    g_pgdir.item[i] = (pg_t) ((uint32_t) pg_tbl_addr & 0xFFFFF000) | (attr & 0x00000FFF); \
}

#define set_page_tbl(i, j, phy_addr, attr) { \
    g_pgtbl[i].item[j] = (pg_t) ((uint32_t) phy_addr & 0xFFFFF000) | (attr & 0x00000FFF); \
}

#define fill_page_tbl(idx, phy_addr_start, attr) { \
    int tbli = 0; int start = (phy_addr_start & 0xFFFFF000); \
    while (tbli < PG_TBL_SIZE) { \
        set_page_tbl(idx, tbli, start + (tbli << __PG_OFFSET__), attr); \
        ++tbli; \
    } \
}

#define enable_paging(pgdir_base) { \
    __asm__ __volatile__ ( \
        "movl %0, %%eax\n\t" \
        "movl %%eax, %%cr3\n\t" \
        "movl %%cr0, %%eax\n\t" \
        "orl  $0x80000000, %%eax\n\t" \
        "movl %%eax, %%cr0\n\t" :: "a"(pgdir_base) \
    ); \
}

void setup_paging(void) {
    memset(&g_pgdir, 0, sizeof(pgdir_t));
    
    set_page_dir(0, logic2phys(&g_pgtbl[0]), 0x3);
    set_page_dir(KERNEL_DIR_SIDX, logic2phys(&g_pgtbl[0]), 0x3);

    fill_page_tbl(0, 0x0, 0x03); 
    /* pg0 0 - 4M */

    enable_paging(logic2phys(&g_pgdir));
}
