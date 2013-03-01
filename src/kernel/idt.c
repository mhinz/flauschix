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
#include <string.h>
#include <common.h>
#include <idt.h>

idt_t       idt;
idt_entry_t idt_entries[0x100];

static void
set_entry(u8 num, u32 base, u16 sel, u8 flags)
{
    idt_entries[num].base_low  = (base & 0xffff);
    idt_entries[num].base_high = ((u16)(base >> 16) & 0xffff);
    idt_entries[num].selector  = sel;
    idt_entries[num].reserved  = 0;
    idt_entries[num].flags     = flags;  // flags | 0x60 for user-mode
}

void
idt_init(void)
{
    size_t size = sizeof (idt_entry_t) * 256;

    idt.limit = (u16)((u16)size - 1);
    idt.base  = (u32)&idt_entries;

    memset(&idt_entries, 0, size);

    /*
     * remap IRQs
     */

    // start initialization sequence (cascade mode)
    outb(PIC_MASTER_CMD,  0x11);
    outb(PIC_SLAVE_CMD,   0x11);

    outb(PIC_MASTER_DATA, 0x20);
    outb(PIC_SLAVE_DATA,  0x28);
    outb(PIC_MASTER_DATA, 0x04);
    outb(PIC_SLAVE_DATA,  0x02);
    outb(PIC_MASTER_DATA, 0x01);
    outb(PIC_SLAVE_DATA,  0x01);
    outb(PIC_MASTER_DATA, 0x0);
    outb(PIC_SLAVE_DATA,  0x0);

    /*
     * set IDT entries
     */

    // CPU interrupts
    set_entry(0,  (u32)isr0,  0x08, 0x8e);
    set_entry(1,  (u32)isr1,  0x08, 0x8e);
    set_entry(2,  (u32)isr2,  0x08, 0x8e);
    set_entry(3,  (u32)isr3,  0x08, 0x8e);
    set_entry(4,  (u32)isr4,  0x08, 0x8e);
    set_entry(5,  (u32)isr5,  0x08, 0x8e);
    set_entry(6,  (u32)isr6,  0x08, 0x8e);
    set_entry(7,  (u32)isr7,  0x08, 0x8e);
    set_entry(8,  (u32)isr8,  0x08, 0x8e);
    set_entry(9,  (u32)isr9,  0x08, 0x8e);
    set_entry(10, (u32)isr10, 0x08, 0x8e);
    set_entry(11, (u32)isr11, 0x08, 0x8e);
    set_entry(12, (u32)isr12, 0x08, 0x8e);
    set_entry(13, (u32)isr13, 0x08, 0x8e);
    set_entry(14, (u32)isr14, 0x08, 0x8e);
    set_entry(15, (u32)isr15, 0x08, 0x8e);
    set_entry(16, (u32)isr16, 0x08, 0x8e);
    set_entry(17, (u32)isr17, 0x08, 0x8e);
    set_entry(18, (u32)isr18, 0x08, 0x8e);
    set_entry(19, (u32)isr19, 0x08, 0x8e);
    set_entry(20, (u32)isr20, 0x08, 0x8e);
    set_entry(21, (u32)isr21, 0x08, 0x8e);
    set_entry(22, (u32)isr22, 0x08, 0x8e);
    set_entry(23, (u32)isr23, 0x08, 0x8e);
    set_entry(24, (u32)isr24, 0x08, 0x8e);
    set_entry(25, (u32)isr25, 0x08, 0x8e);
    set_entry(26, (u32)isr26, 0x08, 0x8e);
    set_entry(27, (u32)isr27, 0x08, 0x8e);
    set_entry(28, (u32)isr28, 0x08, 0x8e);
    set_entry(29, (u32)isr29, 0x08, 0x8e);
    set_entry(30, (u32)isr30, 0x08, 0x8e);
    set_entry(31, (u32)isr31, 0x08, 0x8e);
    // PIC interrupts
    set_entry(32, (u32)irq0,  0x08, 0x8e);
    set_entry(33, (u32)irq1,  0x08, 0x8e);
    set_entry(34, (u32)irq2,  0x08, 0x8e);
    set_entry(35, (u32)irq3,  0x08, 0x8e);
    set_entry(36, (u32)irq4,  0x08, 0x8e);
    set_entry(37, (u32)irq5,  0x08, 0x8e);
    set_entry(38, (u32)irq6,  0x08, 0x8e);
    set_entry(39, (u32)irq7,  0x08, 0x8e);
    set_entry(40, (u32)irq8,  0x08, 0x8e);
    set_entry(41, (u32)irq9,  0x08, 0x8e);
    set_entry(42, (u32)irq10, 0x08, 0x8e);
    set_entry(43, (u32)irq11, 0x08, 0x8e);
    set_entry(44, (u32)irq12, 0x08, 0x8e);
    set_entry(45, (u32)irq13, 0x08, 0x8e);
    set_entry(46, (u32)irq14, 0x08, 0x8e);
    set_entry(47, (u32)irq15, 0x08, 0x8e);

    idt_flush((u32)&idt);

    __asm__ volatile ("sti");
}
