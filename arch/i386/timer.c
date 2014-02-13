#include <sys/type.h>
#include <arch/i386/timer.h>
#include <sys/irpts.h>
#include <sys/protect.h>
#include <arch/i386/8259a.h>
#include <arch/i386/io.h>
#include <lib/kstdio.h>

extern void reserved();

void setup_timer() {
  uint16_t rate = (uint16_t) CLOCK_RATE * CLOCK_INTERVAL / SECOND;
  
  // 向8253 写入控制命令
  // 设置二进制，模式3，选择0通道，先写低字节，后写高字节
  out_byte(CONTROL, BINARY | MODE3 | CHANNEL0 | RW_LMSB);

  // 高字节
  out_byte(COUNTER0, rate & 0xFF);
  out_byte(COUNTER0, rate >> 8);
  
  // 0x20 对应 IRQ0
  set_idt(0x20, (uint32_t) &reserved, kernel_code_selector, IDT_DPL0 | IDT_IRPT);
  out_byte(PIC1_DATA, 0);
  out_byte(PIC2_DATA, 0);
  io_delay();

}
