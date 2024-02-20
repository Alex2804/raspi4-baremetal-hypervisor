#ifndef SPSR_ELX_H
#define SPSR_ELX_H

#include "register.h"

typedef register_aarch64_t spsr_elx_t;

#define SPSR_ELX_EXECUTION_STATE_BIT 4
#define SPSR_ELX_EXCEPTION_LEVEL_BIT 2
#define SPSR_ELX_STACK_POINTER_SELECTION_BIT 0

spsr_elx_t spsr_elx_create_for_el_change(int target_el, int to_aarch32, int uses_sp_elx);

int spsr_elx_get_execution_state(spsr_elx_t spsr);
int spsr_elx_get_exception_level(spsr_elx_t spsr);
int spsr_elx_get_stack_pointer_selection(spsr_elx_t spsr);

#endif //SPSR_ELX_H
