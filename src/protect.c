#include <sys/protect.h>

#define GDT_OFFSET 128
struct gdt_entry    gdts[GDT_OFFSET];
struct gdt_ptr      gdtptr  = {0, GDT_OFFSET};

#define IDT_OFFSET  256
struct gate_entry   idts[IDT_OFFSET];

static void set_gdt(uint32_t idx, uint32_t base, uint16_t limit, uint16_t attr) 
{
    gdts[idx].base_low = base & 0x0FFFF;
    gdts[idx].base_mid = (base >> 16) & 0x0FF;
    gdts[idx].base_high = (base >> 24) & 0x0FF;

}

void setup_gdt() {
    set_gdt(SEG_DUMMY, 0, 0, 0); // DUMMY GDT

}

static void set_idt(uint32_t idx, uint32_t base, uint16_t selector, uint8_t flags) {

}

void setup_idt() {

}
