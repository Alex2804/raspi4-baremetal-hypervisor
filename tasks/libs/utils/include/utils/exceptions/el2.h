#ifndef EXCEPTIONS_EL2_H
#define EXCEPTIONS_EL2_H

#define STACK_FRAME_SIZE        272 // size of all saved registers in bytes

#define EXCEPTION_SYNC_EL2T       0
#define EXCEPTION_IRQ_EL2T        1
#define EXCEPTION_FIQ_EL2T        2
#define EXCEPTION_ERROR_EL2T      3

#define EXCEPTION_SYNC_EL2H       4
#define EXCEPTION_IRQ_EL2H        5
#define EXCEPTION_FIQ_EL2H        6
#define EXCEPTION_ERROR_EL2H      7

#define EXCEPTION_SYNC_EL1_64     8
#define EXCEPTION_IRQ_EL1_64      9
#define EXCEPTION_FIQ_EL1_64      10
#define EXCEPTION_ERROR_EL1_64    11

#define EXCEPTION_SYNC_EL1_32     12
#define EXCEPTION_IRQ_EL1_32      13
#define EXCEPTION_FIQ_EL1_32      14
#define EXCEPTION_ERROR_EL1_32    15


// Exception syndrom register
#define ESR_EL2_EC              (0b111111 << 26) // bits 26-31
#define ESR_EL2_ISS             0xFFFFFF // bits 0-24


// Exception classes
#define ESR_EL2_EC_HVC_AARCH64  (0b010110 << 26)



#ifndef __ASSEMBLER__

#include "utils/registers/register.h"

extern void(*el2_exception_handlers[16])();

void init_el2_vectors();

register_aarch64_t get_esr_el2();
register_aarch64_t get_elr_el2();

#endif

#endif //EXCEPTIONS_EL2_H
