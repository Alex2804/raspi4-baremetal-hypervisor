#ifndef EXCEPTIONS_EL2_H
#define EXCEPTIONS_EL2_H

#define STACK_FRAME_SIZE        272 // size of all saved registers in bytes

#define SYNC_INVALID_EL2T       0
#define IRQ_INVALID_EL2T        1
#define FIQ_INVALID_EL2T        2
#define ERROR_INVALID_EL2T      3

#define SYNC_INVALID_EL2H       4
#define IRQ_INVALID_EL2H        5
#define FIQ_INVALID_EL2H        6
#define ERROR_INVALID_EL2H      7

#define SYNC_INVALID_EL1_64     8
#define IRQ_INVALID_EL1_64      9
#define FIQ_INVALID_EL1_64      10
#define ERROR_INVALID_EL1_64    11

#define SYNC_INVALID_EL1_32     12
#define IRQ_INVALID_EL1_32      13
#define FIQ_INVALID_EL1_32      14
#define ERROR_INVALID_EL1_32    15


// Exception syndrom register
#define ESR_EL2_EC              (0b111111 << 26) // bits 26-31
#define ESR_EL2_ISS             0xFFFFFF // bits 0-24


// Exception classes
#define ESR_EL2_EC_HVC_AARCH64  (0b010110 << 26)



#ifndef __ASSEMBLER__

void init_el2_vectors();

#endif

#endif //EXCEPTIONS_EL2_H
