#ifndef _ISR_H_
#define _ISR_H_

#include <sys/system.h>

#define cli() __asm__ __volatile__ ("cli" ::)
#define sti() __asm__ __volatile__ ("sti" ::)
#define nop() __asm__ __volatile__ ("nop" ::)

void _do_divide0_error(struct irpt_regs* regs);
void _do_debug(struct irpt_regs* regs);
void _do_nmi(struct irpt_regs* regs);
void _do_debug_break(struct irpt_regs* regs);

void setup_irqts();
#endif
