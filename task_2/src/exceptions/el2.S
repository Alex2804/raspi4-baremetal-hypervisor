#include "exceptions/el2.h"

.text

.globl init_el2_vectors
init_el2_vectors:
    adr x0, vectors
    msr vbar_el2, x0
    ret

.macro handle_invalid_entry type
    hypervisor_entry
	mov	x0, #\type
	mrs	x1, esr_el2
	mrs	x2, elr_el2
	bl	show_invalid_entry_message
.endm

.macro	hypervisor_entry
    // set stack pointer to use SP_EL0
    msr     SPSel, #0

	sub	sp, sp, #STACK_FRAME_SIZE
	stp	x0, x1, [sp, #16 * 0]
	stp	x2, x3, [sp, #16 * 1]
	stp	x4, x5, [sp, #16 * 2]
	stp	x6, x7, [sp, #16 * 3]
	stp	x8, x9, [sp, #16 * 4]
	stp	x10, x11, [sp, #16 * 5]
	stp	x12, x13, [sp, #16 * 6]
	stp	x14, x15, [sp, #16 * 7]
	stp	x16, x17, [sp, #16 * 8]
	stp	x18, x19, [sp, #16 * 9]
	stp	x20, x21, [sp, #16 * 10]
	stp	x22, x23, [sp, #16 * 11]
	stp	x24, x25, [sp, #16 * 12]
	stp	x26, x27, [sp, #16 * 13]
	stp	x28, x29, [sp, #16 * 14]

	mrs	x22, elr_el2 // backup exception link register
	mrs	x23, spsr_el2 // backup saved program status register

	stp	x30, x22, [sp, #16 * 15] // x30 is the link register
	str	x23, [sp, #16 * 16]
.endm

.macro	hypervisor_exit
    // set stack pointer to use SP_EL0
    msr     SPSel, #0

	ldr	x23, [sp, #16 * 16]
	ldp	x30, x22, [sp, #16 * 15]

	msr	elr_el2, x22
	msr	spsr_el2, x23

    // backup the registers
	ldp	x0, x1, [sp, #16 * 0]
	ldp	x2, x3, [sp, #16 * 1]
	ldp	x4, x5, [sp, #16 * 2]
	ldp	x6, x7, [sp, #16 * 3]
	ldp	x8, x9, [sp, #16 * 4]
	ldp	x10, x11, [sp, #16 * 5]
	ldp	x12, x13, [sp, #16 * 6]
	ldp	x14, x15, [sp, #16 * 7]
	ldp	x16, x17, [sp, #16 * 8]
	ldp	x18, x19, [sp, #16 * 9]
	ldp	x20, x21, [sp, #16 * 10]
	ldp	x22, x23, [sp, #16 * 11]
	ldp	x24, x25, [sp, #16 * 12]
	ldp	x26, x27, [sp, #16 * 13]
	ldp	x28, x29, [sp, #16 * 14]
	add	sp, sp, #STACK_FRAME_SIZE
	eret
.endm

.macro	ventry	label
	.align	7
	b	\label
.endm

.align	11
vectors:
	ventry	sync_invalid_el2t           // Synchronous EL2t
	ventry	irq_invalid_el2t            // IRQ EL2t
	ventry	fiq_invalid_el2t            // FIQ EL2t
	ventry	error_invalid_el2t          // Error EL2t

	ventry	sync_invalid_el2h           // Synchronous EL2h
	ventry	irq_invalid_el2h            // IRQ EL2h
	ventry	fiq_invalid_el2h            // FIQ EL2h
	ventry	error_invalid_el2h          // Error EL2h

	ventry	sync_el1_64                 // Synchronous 64-bit EL1
	ventry	irq_invalid_el1_64          // IRQ 64-bit EL1
	ventry	fiq_invalid_el1_64          // FIQ 64-bit EL1
	ventry	error_invalid_el1_64        // Error 64-bit EL1

	ventry	sync_invalid_el1_32         // Synchronous 32-bit EL1
	ventry	irq_invalid_el1_32          // IRQ 32-bit EL1
	ventry	fiq_invalid_el1_32          // FIQ 32-bit EL1
	ventry	error_invalid_el1_32        // Error 32-bit EL1

sync_invalid_el2t:
    handle_invalid_entry SYNC_INVALID_EL2T

irq_invalid_el2t:
    handle_invalid_entry IRQ_INVALID_EL2T

fiq_invalid_el2t:
    handle_invalid_entry FIQ_INVALID_EL2T

error_invalid_el2t:
    handle_invalid_entry ERROR_INVALID_EL2T

sync_invalid_el2h:
    handle_invalid_entry SYNC_INVALID_EL2H

irq_invalid_el2h:
    handle_invalid_entry IRQ_INVALID_EL2H

fiq_invalid_el2h:
    handle_invalid_entry FIQ_INVALID_EL2H

error_invalid_el2h:
    handle_invalid_entry ERROR_INVALID_EL2H

sync_el1_64:
    hypervisor_entry
	mrs	x0, esr_el2 // exception syndrom register as first argument
	mrs	x1, elr_el2 // exception link register as second argument
    bl handle_sync_el1_64
    hypervisor_exit

irq_invalid_el1_64:
    handle_invalid_entry IRQ_INVALID_EL1_64

fiq_invalid_el1_64:
    handle_invalid_entry FIQ_INVALID_EL1_64

error_invalid_el1_64:
    handle_invalid_entry ERROR_INVALID_EL1_64

sync_invalid_el1_32:
    handle_invalid_entry SYNC_INVALID_EL1_32

irq_invalid_el1_32:
    handle_invalid_entry IRQ_INVALID_EL1_32

fiq_invalid_el1_32:
    handle_invalid_entry FIQ_INVALID_EL1_32

error_invalid_el1_32:
    handle_invalid_entry ERROR_INVALID_EL1_32
