/*
   CopyRight
   */
#include <kstdio.h>
#include <kstdlib.h>

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


/* Put the character C on the screen. */
void
putchar (int c)
{
    if (c == '\n' || c == '\r')
    {
newline:
        xpos = 0;
        ypos++;
        if (ypos >= LINES)
            ypos = 0;
        return;
    }

    *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

    xpos++;
    if (xpos >= COLUMNS)
        goto newline;
}

/* Clear the screen and initialize VIDEO, XPOS and YPOS. */
void
cls (void)
{
    int i;

    video = (unsigned char *) VIDEO;

    for (i = 0; i < COLUMNS * LINES * 2; i++)
        *(video + i) = 0;

    xpos = 0;
    ypos = 0;
}
