/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

.section ".text.boot"

.global _start

_start:
    // read cpu id
    mrs     x1, mpidr_el1
    and     x1, x1, 0x3

    // branch to main_core if cpu id == 0, else keep running -> slave core
    cbz     x1, main_core

    // set run address of slave core to 0
slave_core:  // cpu id != 0 -> slave core
    wfe  // wait for event
    // read cpu id
    mrs     x1, mpidr_el1
    and     x1, x1, 0x3
    // get address of slave function (cpu id in x1)
    lsl     x1, x1, #3
    adr     x2, spin_core_0  // address of spin core 0
    add     x3, x2, x1
    ldr     x4, [x3]
    cbz     x4, slave_core  // if address is 0, wait for next event
    // if address is not 0, call slave function
    bl      setup_stacks  // setup fresh stack first
    blr     x4  // call slave function
    b       slave_core  // wait for next event


main_core:  // cpu id == 0 -> main core
    // setup stacks for main core
    bl      setup_stacks
    // start cores 1 to 3
    mov     x1, 0xd8  // address of spin core 0
    ldr     x2, =_start  // address where the cores should start
    str     x2, [x1, #8]  // store start address for core 1
    str     x2, [x1, #16]  // store start address for core 2
    str     x2, [x1, #24]  // store start address for core 3
    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
clear_next_bss_byte:
    cbz     w2, main_function
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, clear_next_bss_byte
main_function:
    bl      main  // jump to C code, should not return
    b       hang_loop  // for failsafe, halt this core


setup_stacks:  // requires x1 to contain cpu id
    mrs     x1, mpidr_el1
    and     x1, x1, 0x3
    // setup el0 stack
    ldr     x2, = __EL0_stack__  // load address of el0 stack
    sub     x2, x2, x1, lsl #10   // add cpu id * 1024 to stack address
    msr     sp_el0, x2
    // setup el1 stack
    ldr     x2, = __EL1_stack__  // load address of el1 stack
    sub     x2, x2, x1, lsl #12   // add cpu id * 4096 to stack address
    msr     sp_el1, x2
    // setup el2 stack
    ldr     x2, = __EL2_stack__  // load address of el2 stack
    sub     x2, x2, x1, lsl #12   // add cpu id * 4096 to stack address
    mov     sp, x2
    ret


hang_loop:
    b       hang_loop

// Not really placed at 0xd8, but at 0x800d8 -> 0xd8 should be reserved for startup code
.org 0xd8
.global spin_core_0
spin_core_0:
        .quad 0

.org 0xe0
.global spin_core_1
spin_core_1:
        .quad 0

.org 0xe8
.global spin_core_2
spin_core_2:
        .quad 0

.org 0xf0
.global spin_core_3
spin_core_3:
        .quad 0
