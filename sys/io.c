#include <sys/io.h>
#include "lib/kstdio.h"

void
io_delay(void) {
    __asm__ __volatile__ ( 
            "nop\n\t" 
            "nop\n\t" 
            "nop\n\t" 
            "nop\n\t" 
            ); 
}


uint8_t in_byte(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "dN" (port));
    return ret;
}


void out_byte(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN"(port), "a"(val));
}
