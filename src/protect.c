#include <sys/protect.h>

#define GDT_OFFSET 128
#define GDT_KCODE  1
struct gdt_entry    gdts[GDT_OFFSET];
struct gdt_ptr      gdtptr;
