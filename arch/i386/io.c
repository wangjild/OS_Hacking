#include "io.h"

void
io_delay(void) {
    __asm__ __volatile__ ( 
        "nop\n\t" 
        "nop\n\t" 
        "nop\n\t" 
        "nop\n\t" 
        "ret\n\t"
   ); 
}
