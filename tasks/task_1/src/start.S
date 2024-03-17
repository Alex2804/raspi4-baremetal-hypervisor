.section ".text.boot"

.global _start

_start:
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, 0x3
    cmp     x1, #0
    beq     2f
    // cpu id != 0, stop
1:  wfe
    b       hang_loop
2:  // cpu id == 0

    // set stack pointer to use SP_EL0
    msr     SPSel, #0
    // set top of stack just before our code (stack grows to a lower address per AAPCS64)
    ldr     x1, =_start
    mov     sp, x1

    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b

    // jump to C code, should not return
4:  bl      main
    // for failsafe, halt this core too
    b       hang_loop

hang_loop:
    b       hang_loop