#ifndef _ISR_H_
#define _ISR_H_

void _do_divide0_error(uint32_t esp, uint32_t errcode) {
    FATAL("INT 0: divide error!", esp, errcode);
}

void setup_isrs();
#endif
