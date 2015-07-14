#ifndef _ISR_H_
#define _ISR_H_

#include <sys/system.h>

#define cli() __asm__ __volatile__ ("cli" ::)
#define sti() __asm__ __volatile__ ("sti" ::)
#define nop() __asm__ __volatile__ ("nop" ::)

struct isr_regs
{
    uint32_t    fs, es, ds; /* 压入上一个段。 */
    uint32_t    ebp, esi, edi, edx, ecx; /* 由push 压入*/
    uint32_t    isr_num, errcode;   /* 由exchange 压入*/
    uint32_t    eip, cs, eflags, uesp, ss; /* 由处理器自动压入。 */ 
};

void _do_divide0_error(uint32_t errcode, struct isr_regs* regs);
void _do_debug(uint32_t errcode, struct isr_regs* regs);
void _do_nmi(uint32_t errcode, struct isr_regs* regs);
void _do_debug_break(uint32_t errcode, struct isr_regs* regs);
void _do_overflow(uint32_t errcode, struct isr_regs* regs);
void _do_bounds_check(uint32_t errcode, struct isr_regs* regs);
void _do_invalid_op(uint32_t errcode, struct isr_regs* regs);
void _do_device_fail(uint32_t errcode, struct isr_regs* regs);
void _do_double_fault(uint32_t errcode, struct isr_regs* regs);
void _do_cop_segment(uint32_t errcode, struct isr_regs* regs);
void _do_tss_inval(uint32_t errcode, struct isr_regs* regs);
void _do_segment_unpresent(uint32_t errcode, struct isr_regs* regs);
void _do_stack_segment(uint32_t errcode, struct isr_regs* regs);
void _do_general_protection(uint32_t errcode, struct isr_regs* regs);
void _do_page_fault(uint32_t errcode, struct isr_regs* regs);
void _do_reserved(uint32_t errcode, struct isr_regs* regs);
void _do_cop_error(uint32_t errcode, struct isr_regs* regs);

void setup_isr();

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void exception_handler();

#endif
