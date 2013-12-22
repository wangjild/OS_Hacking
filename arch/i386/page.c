#include <sys/type.h>
#include <arch/i386/page.h>

pg_t g_pgdir[PG_DIR_SIZE] __pgdir;
pg_t g_pg0[PG_TBL_SIZE] __pgtbl;

#define hh2phys(viraddr) (((void*) viraddr) + 0x0 - KERNEL_OFFSET)

void setup_paging(void) {
    void *pgdirptr = hh2phys(g_pgdir);
    void *pg0ptr = hh2phys(g_pg0);
    int k;
    for(k = 0; k < PG_TBL_SIZE; ++k) {
        g_pg0[k] = (uint32_t) (k * (1<<__PG_OFFSET__)) | 0x3;
        g_pgdir[k] = (uint32_t) 0;
    }

    g_pgdir[0] = (uint32_t) pg0ptr | 0x03;
    g_pgdir[KERNEL_DIR_SIDX] = (uint32_t) pg0ptr | 0x03;

    __asm__ __volatile__ ( 
        "movl %0, %%eax\n\t" 
        "movl %%eax, %%cr3\n\t" 
        "movl %%cr0, %%eax\n\t" 
        "orl  $0x80000000, %%eax\n\t" 
        "movl %%eax, %%cr0\n\t" :: "m"(pgdirptr)
    ); 
}
