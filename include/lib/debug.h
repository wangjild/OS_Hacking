#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <lib/kstdio.h>
#include <isr.h>

#define FATAL(str, errcode, regs) { \
    printk("[FATAL] %s\n", str); \
    printk("EIP:    %x:%x\nEFLAGS: %x\nESP:    %x:%x\n", \
            regs->cs, regs->eip, regs->eflags, regs->es, regs->uesp); \
    printk("ES:     %x\n", regs->es); \
    __asm__ __volatile__ ("hlt\r\n"); \
}

#define INFO(str) { \
    printk("[INFO] %s", str); \
}

#endif
