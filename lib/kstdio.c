/*
 *  Copyright (C) 2013  Wang Jing <wangjild@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *   Author:        wangjild <wangjild@gmail.com>
 *   Github:        https://github.com/wangjild/OS_Hacking
 *   File:          kstdio.c
 *   Lauguage:      c/cpp
 *   Date:          13-12-15 20:09:48
 *   Descripton:    
 */

#include <sys/screen.h>
#include <lib/kstdio.h>
#include <lib/kstdlib.h>

#include <sys/io.h>

/* Variables. */
/* Save the X position. */
static int xpos = 0;
/* Save the Y position. */
static int ypos = 0;
/* Point to the video memory. */
static volatile uint16_t *video = (uint16_t*) VIDEO;

/* Format a string and print it on the screen, just like the libc
   function printf. */
void
printk (const char *format, ...)
{
    char **arg = (char **) &format;
    int c;
    char buf[20];

    arg++;

    while ((c = *format++) != 0)
    {
        if (c != '%')
            putchar (c);
        else
        {
            char *p;

            c = *format++;
            switch (c)
            {
                case 'd':
                case 'u':
                case 'x':
                    itoa (buf, c, *((int *) arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (! p)
                        p = "(null)";
string:
                    while (*p)
                        putchar (*p++);
                    break;

                default:
                    putchar (*((int *) arg++));
                    break;
            }
        }
    }
}

static void 
cursor(void) {
    uint16_t loc = ypos * COLUMNS + xpos;
    out_byte(0x3D4, 0x0F);
    out_byte(0x3D5, loc & 0xFF);
    
    out_byte(0x3D4, 0x0E);
    out_byte(0x3D5, (loc >> 8)&0xFF);
}


/* Scrolls the screen by on line */
static void 
scroll(void) {
    uint8_t attr = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | attr << 8;

    if (ypos >= LINES+1) {
        // Move the current line up
        for (int i = 0 * COLUMNS; i < LINES * COLUMNS; ++i) {
            video[i] = video[i+COLUMNS];
        }

        // The last line should be blank
        for (int i = LINES * COLUMNS; i < (LINES+1) * COLUMNS; ++i) {
            video[i] = blank;
        }

        ypos = LINES;
    }
}

/* Put the character C on the screen. */
void
putchar (int c)
{
    uint8_t backColour = C_BLANK;
    uint8_t foreColour = C_GREEN;

    uint8_t attrByte = (backColour << 4) | (foreColour & 0x0F);
    uint16_t attr = attrByte << 8;

    switch (c) {
        case 0x08:
            xpos ? --xpos: 0;
            break;
        case 0x09:
            xpos = (xpos + 8) & ~(8 - 1);
            break;
        case '\r':
            xpos = 0;
            break;
        case '\n':
            xpos = 0; ++ypos;
            break;
    }

    if (c >= ' ') {
        video[ypos*COLUMNS + xpos] = c | attr;
        ++xpos;
    }

    if (xpos >= COLUMNS) {
        ++ypos; xpos = 0; 
    }


    scroll();
    cursor();
}

/* Clear the screen and initialize VIDEO, XPOS and YPOS. */
void
cls (void)
{
    int i;

    for (i = 0; i < COLUMNS * LINES * 2; i++)
        *(video + i) = 0;

    xpos = 0; ypos = 0;
    cursor();
}
