#ifndef _KSTDLIB_H_
#define _KSTDLIB_H_

#include <sys/type.h>

uint8_t in_byte(uint16_t port);
void out_byte(uint16_t port, uint8_t val);

void itoa(char *buf, int base, int d);

#endif
