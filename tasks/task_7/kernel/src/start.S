.section ".text.boot"

.global _start

_start:
    // read cpu id
    mrs     x0, mpidr_el1
    and     x0, x0, 0x3

    // branch to main_core if cpu id == 0, else keep running -> slave core
    cbz     x0, main_core

    // set run address of slave core to 0
slave_core:  // cpu id != 0 -> slave core
    wfe  // wait for event
    // read cpu id
    mrs     x0, mpidr_el1
    and     x0, x0, 0x3
    // get address of slave function (cpu id in x0)
    adr     x1, __spin_core_0  // address of spin core 0
    add     x1, x1, x0, lsl #3  // spin_core_x = spin_core_0 address + (core_id * 8)
    ldr     x2, [x1]  // loads the target address which lies at memory position of x1 into x2
    cbz     x2, slave_core  // if target (jump) address is 0, wait for next event
    // if address is not 0
    str     xzr, [x1]  // resets the core that it doesn't loop over the provided function
    bl      setup_stacks  // setup fresh stack first
    blr     x2  // call slave function
    b       slave_core  // wait for next event


main_core:  // cpu id == 0 -> main core
    // start cores 1 to 3
    mov     x1, 0xd8  // address of spin core 0
    ldr     x2, =_start  // address where the cores should start
    str     x2, [x1, #8]  // store start address for core 1
    str     x2, [x1, #16]  // store start address for core 2
    str     x2, [x1, #24]  // store start address for core 3
    ldr     x1, =__spin_core_0  // address of fake spin core 0
    str     xzr, [x1]  // store start address for core 0 after stack setup
    str     xzr, [x1, #8]  // store start address for core 1 after stack setup
    str     xzr, [x1, #16]  // store start address for core 2 after stack setup
    str     xzr, [x1, #24]  // store start address for core 3 after stack setup
    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
clear_next_bss_byte:
    cbz     w2, main_function
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, clear_next_bss_byte
main_function:
    bl      setup_stacks // setup stacks for main core
    bl      main  // jump to C code, should not return
    b       hang_loop  // for failsafe, halt this core


setup_stacks:
    // read cpu id
    mrs     x0, mpidr_el1
    and     x0, x0, 0x3
    // setup el0 stack
    ldr     x1, = __EL0_stack__  // load address of el0 stack
    sub     x1, x1, x0, lsl #10   // add cpu id * 1024 to stack address
    msr     sp_el0, x1
    // setup el1 stack
    ldr     x1, = __EL1_stack__  // load address of el1 stack
    sub     x1, x1, x0, lsl #12   // add cpu id * 4096 to stack address
    msr     sp_el1, x1
    // setup el2 stack
    ldr     x1, = __EL2_stack__  // load address of el2 stack
    sub     x1, x1, x0, lsl #12   // add cpu id * 4096 to stack address
    mov     sp, x1
    ret


hang_loop:
    b       hang_loop
