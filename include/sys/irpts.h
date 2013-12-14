#ifndef _ISR_H_
#define _ISR_H_

#define cli() __asm__ __volatile__ ("cli" ::)
#define sti() __asm__ __volatile__ ("sti" ::)
#define nop() __asm__ __volatile__ ("nop" ::)

void setup_irqts();
#endif
