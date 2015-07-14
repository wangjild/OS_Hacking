#include <lib/kstdlib.h>
#include <sys/timer.h>
#include <protect.h>
#include <lib/debug.h>
#include <isr.h>
#include <sys/io.h>


static int tick = 0;

void _do_timer(uint32_t errcode, struct isr_regs* regs) {
    INFO("int 0x20: timer interrupt");
    PIC_sendEOI(0);
}

void init_timer_irq() {
  set_irq_gate(0, (uint32_t) &_do_timer);
}

void setup_timer() {
    
  init_timer_irq();
    
  uint16_t rate = (uint16_t) (CLOCK_RATE * CLOCK_INTERVAL / SECOND);

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
}
