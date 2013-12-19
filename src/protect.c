#include <sys/protect.h>
#include <arch/i386/page.h>

#define GDT_OFFSET 128
#define SEG_KERNEL_C    SEG_KCODE_FLAT
#define SEG_KERNEL_D    SEG_KDATA_FLAT
const uint16_t gdt_offset = GDT_OFFSET;
const uint16_t kernel_code_selector = SEG_KERNEL_C << 3;
const uint16_t kernel_data_selector = SEG_KERNEL_D << 3;

struct gdt_entry    gdts[GDT_OFFSET];
struct gdt_ptr      gdtptr  = {0, GDT_OFFSET};

#define phy_gdts_base() ((void*) vir2phys(&gdts[0])) 
#define phy_gdts(i) ((struct gdt_entry*) vir2phys(&gdts[i]))

#define set_gdt(idx, base, limit, attr) \
{ \
    phy_gdts(idx)->base_low = base & 0x0FFFF; \
    phy_gdts(idx)->base_mid = (base >> 16) & 0x0FF; \
    phy_gdts(idx)->base_high = (base >> 24) & 0x0FF; \
    phy_gdts(idx)->limit_low = limit & 0x0FFFF; \
    phy_gdts(idx)->attr_limit_high = ((limit >> 8) & 0x0F00) | attr; \
}

extern void gdt_flush();

#define phy_gdtptr() ((struct gdt_ptr*) vir2phys(&gdtptr))

void setup_gdt() {
    memset(phy_gdts_base(), 0, sizeof(struct gdt_entry) * GDT_OFFSET);

    phy_gdtptr()->base = (uint32_t) phy_gdts_base();
    phy_gdtptr()->limit = sizeof(gdts) - 1;
    
    set_gdt(SEG_DUMMY, 0, 0, 0); // DUMMY GDT
    set_gdt(SEG_KERNEL_C, 0, 0xFFFFFF, SEG_32 | SEG_4K | SEG_CAR); // KERNEL CODE GDT
    set_gdt(SEG_KERNEL_D, 0, 0xFFFFFF, SEG_32 | SEG_4K | SEG_DRW); // KERNEL DATA GDT
    gdt_flush();
}
