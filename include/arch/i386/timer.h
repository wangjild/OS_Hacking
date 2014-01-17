#ifndef _ARCH_I386_TIMER_H_
#define _ARCH_I386_TIMER_H_

#define CLOCK_RATE 1193180
#define CLOCK_INTERVAL 10 // 10 ms
#define SECOND 1000 // 1000ms = 1s

#define COUNTER0 0x40
#define COUNTER1 0x41
#define COUNTER2 0x42
#define CONTROL  0x43

#define CHANNEL0 0x00
#define CHANNEL1 0x40
#define CHANNEL2 0x80
#define CHANNEL3 0xC0

#define RW_LOCK  0x00
#define RW_LSB   0x10
#define RW_MSB   0x20
#define RW_LMSB  0x30

#define MODE0    0x00
#define MODE1    0x02
#define MODE2    0x04
#define MODE3    0x06
#define MODE4    0x08
#define MODE5    0x0A

#define BINARY   0x00
#define BCD      0x01

void setup_timer();

#endif
