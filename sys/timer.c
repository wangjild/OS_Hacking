#include <lib/kstdlib.h>
#include <sys/timer.h>
#include <protect.h>
#include <lib/debug.h>
#include <isr.h>
#include <sys/io.h>


static uint32_t tick = 0;

void _do_irq0(uint32_t errcode, irq_args_t* args) {
   
    ++tick; 
    
    printk("errcode: %d, tick: %d\n", errcode, tick);
    
    PIC_sendEOI(0);
}

extern void irq0();

void init_timer_irq() {
  set_irq_gate(0, (uint32_t) &irq0);
}

void setup_timer(uint16_t hz) {
    
  init_timer_irq();
    
  uint16_t rate = CLOCK_RATE / hz;

  // 向8253 写入控制命令
  // 设置二进制，模式3，选择0通道，先写低字节，后写高字节
  out_byte(CONTROL, BINARY | MODE3 | CHANNEL0 | RW_LMSB);
  io_delay();

  //low
  out_byte(COUNTER0, rate & 0xFF);
  io_delay();

  // high
  out_byte(COUNTER0, (rate >> 8) & 0xFF);
  io_delay();

  /*
  out_byte(PIC1_DATA, 0);
  io_delay();
  out_byte(PIC2_DATA, 0);
  io_delay();
  */

  enable_irq(0);
}
