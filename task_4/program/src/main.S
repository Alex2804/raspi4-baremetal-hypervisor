.section ".text.start"

.global _start

_start:
    stp	x29, x30, [sp, #-16]!           // Save fp and sp
    mov	x29, sp
    bl main
    ldp	x29, x30, [sp], #16             // Restore fp and sp
    ret