extern isr_handler

%macro ISR_NOERR 1
    global isr%1
    isr%1:
        cli
        push    byte 0    ; dummy error code
        push    byte 0    ; interrupt number
        jmp     isr_stub
%endmacro

%macro ISR 1
    global isr%1
    isr%1:
        cli
        push    byte %1   ; interrupt number
        jmp     isr_stub
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR       8
ISR_NOERR 9
ISR       10
ISR       11
ISR       12
ISR       13
ISR       14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

;
; CPU pushes following registers onto the stack if interrupt is fired:
;
;    - esp
;    - ss
;    - eflags
;    - eip
;    - cs
;
isr_stub:
    pusha               ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov     ax, ds

    mov     ax, 0x10    ; load kernel data segment descriptor
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    call    isr_handler

    pop     eax         ; load original data segment descriptor
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    popa
    add     esp, 8      ; cleans error code and interrupt number
    sti

    iret                ; pops CS, EIP, ...
