#include <sys/type.h>
#include <sys/isr.h>

#define isr(N) void isr##N() { \
    __asm__ __volatile__ ("cli\n\t" :: ); \
    isr_common_stub(N); \
    __asm__ __volatile__ ("iret\n\t" :: ); \
}


extern const uint16_t kernel_data_selector;

void _handler () {}

void isr_common_stub(int isrnum) {
    const uint16_t* skc = &kernel_data_selector;
    uint32_t hand = (uint32_t) &_handler;

    __asm__ __volatile__ (
        "pusha\n\t"
        "push %%ds\n\t"
        "push %%es\n\t"
        "push %%fs\n\t"
        "push %%gs\n\t"
        "mov %0, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%esp, %%eax\n\t"
        "push %%eax\n\t"
        "mov %1, %%eax\n\t"
        "call %%eax\n\t"
        "pop %%eax\n\t"
        "pop %%gs\n\t"
        "pop %%fs\n\t"
        "pop %%es\n\t"
        "pop %%ds\n\t"
        "popa\n\t" :: "m"(skc), "m"(hand)
        );
}

isr(0);
isr(1);
isr(2);
isr(3);

void isrs_setup() 
{
};
