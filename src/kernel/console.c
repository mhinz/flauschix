/*
 * Copyright (c) 2013 Marco Hinz
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the author nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdarg.h>
#include <console.h>
/*#include <common.h>*/

/*
 * See docs/vga.txt for more detailed information
 * about how we access the VGA hardware.
 */

static u8 *vram = (u8 *)0xb8000;
static u32 ret  = 0;
static u8 x     = 0;
static u8 y     = 0;


/*static void*/
/*cursor_move(void)*/
/*{*/
    /*u16 pos = (u16)(y * 80 + x);*/

    /*outb(0x3d4, 14);*/
    /*outb(0x3d5, (u8)(pos >> 8));*/
    /*outb(0x3d4, 15);*/
    /*outb(0x3d5, (u8)pos);*/
/*}*/

static void
kputc(const char c)
{
    if (x >= 80) {
        ++y;
        x = 0;
    }
    else {
        switch (c) {
            case '\n':
                ++y;
            case '\r':
                x = 0;
                goto end;
            case '\b':
                if (x)
                    --x;
                goto end;
            case '\t':
                x = (u8)((x + 8) & ~(8 - 1));
                goto end;
        }
    }

    // scroll by one line if needed
    if (y >= 25) {
        int i = 0;
        for (; i < (2 * 24 * 80); ++i)
            vram[i] = vram[i + 160];
        for (; i < (2 * 25 * 80); ++i)
            vram[i] = 0;
        y = 24;
    }

    u16 off       = (u16)(2 * (y * 80 + x));
    vram[off]     = (u8)c;
    vram[off + 1] = (0 << 4) | (15 & 0x0f);

    ++x;
end:
    ++ret;
}

static void
kputs(const char *s)
{
    while (*s)
        kputc(*s++);
}

static void
kputn(int n, const int base)
{
    if (base > 36)
        return;

    const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    char buf[65];
    char *p = buf + 64;
    *p = '\0';

    do {
        *--p = digits[n % base];
        n /= base;
    } while (n);

    kputs(p);
}

void
vga_clear_screen(void)
{
    for (int i = 0; i < (2 * 25 * 80); ++i)
        vram[i] = 0;

    x = y = 0;
}

u32
vga_printf(const char *fmt, ...)
{
    const char *s;
    int n;
    va_list ap;

    va_start(ap, fmt);
    ret = 0;

    while (*fmt) {
        if (*fmt == '%') {
            ++fmt;

            switch (*fmt) {
                case 's':
                    s = va_arg(ap, char *);
                    kputs(s);
                    break;
                case 'd':
                case 'u':
                    n = va_arg(ap, int);
                    kputn(n, 10);
                    break;
                case 'x':
                case 'p':
                    n = va_arg(ap, int);
                    kputn(n, 16);
                    break;
                case '%':
                    kputc('%');
                    break;
                case '\0':
                    goto out;
                default:
                    kputc(*fmt);
                    break;
            }
        }
        else
            kputc(*fmt);
        ++fmt;
    }

out:
    va_end(ap);

    return ret;
}
