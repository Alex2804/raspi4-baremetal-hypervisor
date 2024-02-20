#include "utils/registers/spsr_elx.h"

spsr_elx_t spsr_elx_create_for_el_change(int target_el, int to_aarch32, int uses_sp_elx) {
    return to_aarch32 << SPSR_ELX_EXECUTION_STATE_BIT | target_el << SPSR_ELX_EXCEPTION_LEVEL_BIT |
        uses_sp_elx << SPSR_ELX_STACK_POINTER_SELECTION_BIT;
}

int spsr_elx_get_execution_state(spsr_elx_t spsr) { return (spsr >> SPSR_ELX_EXECUTION_STATE_BIT) & 1; }
int spsr_elx_get_exception_level(spsr_elx_t spsr) { return (spsr >> SPSR_ELX_EXCEPTION_LEVEL_BIT) & 3; }
int spsr_elx_get_stack_pointer_selection(spsr_elx_t spsr) { return (spsr >> SPSR_ELX_STACK_POINTER_SELECTION_BIT) & 1; }
