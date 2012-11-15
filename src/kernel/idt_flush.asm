global idt_flush

idt_flush:
    mov     eax, [esp+4]  ; get pointer to IDT
    lidt    [eax]         ; load new IDT
    ret
