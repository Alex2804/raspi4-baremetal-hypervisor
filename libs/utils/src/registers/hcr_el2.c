#include "utils/registers/hcr_el2.h"

hcr_el2_t hcr_el2_create_from_spsr(spsr_elx_t spsr) {
    return (!spsr_elx_get_execution_state(spsr) & 1) << HCR_EL2_EXECUTION_STATE_BIT;
}
