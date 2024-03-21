#include "utils/exceptions/el2.h"

#include "utils/exceptions/exceptions.h"

void(*el2_exception_handlers[16])();

register_aarch64_t get_esr_el2() {
    register_aarch64_t esr_el2;
    asm volatile("mrs %0, esr_el2" : "=r"(esr_el2));
    return esr_el2;
}

register_aarch64_t get_elr_el2() {
    register_aarch64_t elr_el2;
    asm volatile("mrs %0, elr_el2" : "=r"(elr_el2));
    return elr_el2;
}
