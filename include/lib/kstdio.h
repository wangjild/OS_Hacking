#ifndef _KSTDIO_H_
#define _KSTDIO_H_
/* Some screen stuff. */
/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xB8000
     
/* Variables. */
/* Save the X position. */
static int xpos = 0;
/* Save the Y position. */
static int ypos = 0;
/* Point to the video memory. */
static volatile unsigned char *video = 0;

void putchar (int c);
void printk (const char *format, ...);
void cls(void);

#endif
