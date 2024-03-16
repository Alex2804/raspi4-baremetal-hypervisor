#include "utils/exceptions/exceptions.h"

#include "utils/uart.h"

#include "utils/registers/hcr_el2.h"


static void print_spsr_info(spsr_elx_t spsr) {
    uart_write_string("EL");
    const int el = spsr_elx_get_exception_level(spsr);
    uart_write_long(el);
    uart_write_string(" with SP_EL");
    uart_write_long(spsr_elx_get_stack_pointer_selection(spsr) ? el : 0);
    uart_write_string(" and ");
    uart_write_string(spsr_elx_get_execution_state(spsr) ? "AArch32" : "AArch64");
}


void print_el(const char* message) {
    uart_write_string(message);
    uart_write_long(get_el());
    uart_write_newline();
}

void el_downstep(int current_el, spsr_elx_t spsr, void* eret_addr) {
    uart_write_string("stepping down from EL");
    uart_write_long(current_el);
    uart_write_string(" to ");
    print_spsr_info(spsr);
    uart_write_newline();

    if(current_el == 2) {
        const hcr_el2_t hcr_el2 = hcr_el2_create_from_spsr(spsr);
        asm volatile("msr hcr_el2, %0\n\t"
                     "msr elr_el2, %1\n\t"
                     "msr spsr_el2, %2\n\t"
                     "eret" : : "r"(hcr_el2), "r"(eret_addr), "r"(spsr));
    } else if(current_el == 1) {
        asm volatile("msr elr_el1, %0\n\t"
                     "msr spsr_el1, %1\n\t"
                     "eret" : : "r"(eret_addr), "r"(spsr));
    } else {
        uart_write_string("Stepping down from EL");
        uart_write_long(current_el);
        uart_write_string(" is not supported!\n");
    }
}

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address) {
    uart_write_string("Exception type: ");
    uart_write_long(type);
    uart_write_string("; return address: ");
    uart_write_hex(address);
    uart_write_string("; ESR: ");
    uart_write_hex(esr);
    uart_write_newline();

    volatile int i = 0;
    while(i == 0) {}
}
