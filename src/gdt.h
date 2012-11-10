#pragma once

#include <stdint.h>

/*
 * Wanna hear the the good or the bad message first?
 * Okay, the good one. Grub sets up the GDT for us! And the bad one, you ask?
 * We neither know where nor what it contains. Since we don't want to invoke
 * triple-faults by accidentally overwriting it, we set up the GDT ourselves.
 *
 * Access byte:
 *
 * 0-3:   Type     cs/ds
 * 4:     DT       descriptor type
 * 5-6:   DPL      descriptor prilege type (aka ring)
 * 7:     P        segment present?
 *
 * Granularity byte:
 *
 * 0-3:   SL       segment length
 * 4:     A        available for system use (always zero)
 * 5:     0        always zero
 * 6:     D        operand size (0 = 16 bit, 1 = 32 bit)
 * 7:     G        granularity (0 = 1 byte, 1 = 1 kbyte)
 */

struct gdt_entry {
    u16 limit_low;    // lower 16 bits of limit
    u16 base_low;     // lower 16 bits of base
    u8  base_middle;  // next 8 bits of base
    u8  access;       // access flags
    u8  granularity;  // granularity flags
    u8  base_high;    // upper 8 bits of base
} __attribute__((packed));

typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr {
    u16 limit;        // upper 16 bits of all selector limits
    u32 base;         // address of the first gdt_entry_t
} __attribute__((packed));

typedef struct gdt_ptr gdt_ptr_t;
