#ifndef _ISR_H_
#define _ISR_H_

#include <sys/system.h>

#define cli() __asm__ __volatile__ ("cli" ::)
#define sti() __asm__ __volatile__ ("sti" ::)
#define nop() __asm__ __volatile__ ("nop" ::)

struct irpt_regs
{
    uint32_t    fs, es, ds; /* 压入上一个段。 */
    uint32_t    ebp, esi, edi, edx, ecx; /* 由push 压入*/
    uint32_t    ebx, eax;   /* 由exchange 压入*/
    uint32_t    eip, cs, eflags, uesp, ss; /* 由处理器自动压入。 */ 
};

void _do_divide0_error(uint32_t errcode, struct irpt_regs* regs);
void _do_debug(uint32_t errcode, struct irpt_regs* regs);
void _do_nmi(uint32_t errcode, struct irpt_regs* regs);
void _do_debug_break(uint32_t errcode, struct irpt_regs* regs);
void _do_overflow(uint32_t errcode, struct irpt_regs* regs);
void _do_bounds_check(uint32_t errcode, struct irpt_regs* regs);
void _do_invalid_op(uint32_t errcode, struct irpt_regs* regs);
void _do_device_fail(uint32_t errcode, struct irpt_regs* regs);
void _do_double_fault(uint32_t errcode, struct irpt_regs* regs);
void _do_cop_segment(uint32_t errcode, struct irpt_regs* regs);
void _do_tss_inval(uint32_t errcode, struct irpt_regs* regs);
void _do_segment_unpresent(uint32_t errcode, struct irpt_regs* regs);
void _do_stack_segment(uint32_t errcode, struct irpt_regs* regs);
void _do_general_protection(uint32_t errcode, struct irpt_regs* regs);
void _do_page_fault(uint32_t errcode, struct irpt_regs* regs);
void _do_reserved(uint32_t errcode, struct irpt_regs* regs);
void _do_cop_error(uint32_t errcode, struct irpt_regs* regs);

void setup_irqts();

void divide0_error();
void debug();
void nmi();
void debug_break();
void overflow();
void bounds_check();
void invalid_op();
void device_fail();
void double_fault();
void cop_segment();
void tss_inval();
void segment_unpresent();
void stack_segment();
void general_protection();
void page_fault();
void reserved();
void cop_error();
void exception_handler();

#endif
