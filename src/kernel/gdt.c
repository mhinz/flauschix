/*
 * Copyright (c) 2012 Marco Hinz
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
#include <gdt.h>

extern void gdt_flush(u32);

/*
 * Failing setting up at least the following five entries will lead
 * to a state of considerable discontent:
 *   - null entry
 *   - kernel code segment
 *   - kernel data segment
 *   - user code segment
 *   - user data segment
 */
gdt_entry_t gdtt[5];
gdt_ptr_t   gdtp;

static void
set_entry(u32 num, u32 base, u32 limit, u8 access, u8 gran)
{
    gdtt[num].base_low     = (base & 0xffff);
    gdtt[num].base_middle  = (base >> 16) & 0xff;
    gdtt[num].base_high    = (base >> 24) & 0xff;

    gdtt[num].limit_low    = (limit & 0xffff);
    gdtt[num].granularity  = (limit >> 16) & 0x0f;

    gdtt[num].granularity |= gran & 0xf0;
    gdtt[num].access       = access;
}

void
gdt_init(void)
{
    gdtp.limit = (sizeof (gdt_entry_t) * 5) - 1;
    gdtp.base  = (u32)&gdtt;

    set_entry(0, 0, 0,          0,    0   );  // null segment
    set_entry(1, 0, 0xffffffff, 0x9a, 0xcf);  // kernel code segment
    set_entry(2, 0, 0xffffffff, 0x92, 0xcf);  // kernel data segment
    set_entry(3, 0, 0xffffffff, 0xfa, 0xcf);  // user mode code segment
    set_entry(4, 0, 0xffffffff, 0xf2, 0xcf);  // user mode data segment

    gdt_flush((u32)&gdtp);
}
