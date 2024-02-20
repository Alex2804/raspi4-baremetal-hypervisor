#ifndef HCR_EL_H
#define HCR_EL_H

#include "register.h"
#include "spsr_elx.h"

typedef register_aarch64_t hcr_el2_t;

#define HCR_EL2_EXECUTION_STATE_BIT 31

hcr_el2_t hcr_el2_create_from_spsr(spsr_elx_t spsr);

#endif //HCR_EL_H
