#include <sys/protect.h>

#define GDT_OFFSET 128
const uint16_t gdt_offset = GDT_OFFSET;
const uint16_t kernel_code_selector = SEG_KERNEL_C << 3;
const uint16_t kernel_data_selector = SEG_KERNEL_D << 3;

struct gdt_entry    gdts[GDT_OFFSET];
struct gdt_ptr      gdtptr  = {0, GDT_OFFSET};

#define IDT_OFFSET  256
struct gate_entry   idts[IDT_OFFSET];

static void set_gdt(uint32_t idx, uint32_t base, uint32_t limit, uint16_t attr) 
{
    gdts[idx].base_low = base & 0x0FFFF;
    gdts[idx].base_mid = (base >> 16) & 0x0FF;
    gdts[idx].base_high = (base >> 24) & 0x0FF;
    gdts[idx].limit_low = limit & 0x0FFFF;
    gdts[idx].attr_limit_high = ((limit >> 16) & 0x0F0) | attr;
}
extern void gdt_flush();

void setup_gdt() {
    gdtptr.base = (uint32_t) &gdts;
    gdtptr.limit = GDT_OFFSET;
    
    printk("code selector:%x\n", kernel_code_selector);
    printk("data selector:%x\n", kernel_data_selector);
    set_gdt(SEG_DUMMY, 0, 0, 0); // DUMMY GDT
    set_gdt(SEG_KERNEL_C, 0, 0x0FFFFF, SEG_32 | SEG_4K | SEG_CR); // KERNEL CODE GDT
    set_gdt(SEG_KERNEL_D, 0, 0x0FFFFF, SEG_32 | SEG_4K | SEG_DRW); // KERNEL DATA GDT
    gdt_flush();
}

static void set_idt(uint32_t idx, uint32_t base, uint16_t selector, uint8_t flags) {

}

void setup_idt() {

}
