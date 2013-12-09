#ifndef _KSTDLIB_H_
#define _KSTDLIB_H_

#include <sys/type.h>

void itoa(char *buf, int base, int d);

void out_byte(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a"(val));
}
#endif
