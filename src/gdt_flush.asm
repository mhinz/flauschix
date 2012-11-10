global gdt_flush

gdt_flush:
    mov     eax, [esp+4]  ; get pointer to GDT
    lgdt    [eax]         ; load new GDT pointer

    mov     ax, 0x10      ; offset in GDT to our ds
    mov     ds, ax        ; load all ds selectors
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    jmp     0x08:.flush   ; offset to our cs

.flush:
    ret
