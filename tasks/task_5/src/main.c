#include "utils/uart.h"
#include "utils/multicore.h"
#include "utils/exceptions/el2.h"
#include "utils/exceptions/exceptions.h"

volatile int finished_core = 0;

void handle_sync_el1_64() {
    const unsigned long esr = get_esr_el2();
    const unsigned long elr = get_elr_el2();
    if((esr & ESR_EL2_EC) == ESR_EL2_EC_HVC_AARCH64) {
        uart_write_string("hvc from EL1, we are now at EL");
        uart_write_long(get_el());
        uart_write_newline();
    } else {
        uart_write_string("handle_sync_el1_64 called without known esr -> ");
        show_invalid_entry_message(EXCEPTION_SYNC_EL1_64, esr, elr);
    }
}

void step_downward_to_el0_code() {
    uart_write_string("We are now at EL0\n");

    finished_core = 1;

    volatile int i = 1;
    while(i) {
        asm volatile("nop");
    }
}
void step_down_to_el0() {
    const spsr_elx_t spsr = spsr_elx_create_for_el_change(0, 0, 0);
    el_downstep(get_el(), spsr, &step_downward_to_el0_code); // return to aarch64 el0 with (implicit) sp_el0
}


void step_up_to_el2() {
    print_el("Before hvc instruction, we are at EL");

    asm("hvc #0xf2");

    print_el("After hvc instruction, we are at EL");
}


void step_downward_to_el1_code() {
    print_el("We are now at EL");

    step_up_to_el2();
    step_down_to_el0();
}
void step_down_to_el1() {
    const int el = get_el();
    if(el == 3 || el == 2) {
        const spsr_elx_t spsr = spsr_elx_create_for_el_change(1, 0, 1);
        el_downstep(el, spsr, &step_downward_to_el1_code); // return to aarch64 el1 with sp_el0
    }
}

void slave_core_main() {
    uart_write_string("Hello from core ");
    uart_write_long(get_core_id());
    uart_write_string("!\n");

    init_el2_vectors();
    el2_exception_handlers[EXCEPTION_SYNC_EL1_64] = handle_sync_el1_64;  // set handler for hvc instruction
    step_down_to_el1();
}

void main()
{
    uart_init();
    uart_write_string("Hello from main core (");
    uart_write_long(get_core_id());
    uart_write_string(")!\n");

    uart_write_string("Starting other cores ...\n\n");

    for(int core = 1; core < 4; ++core) {
        finished_core = 0;
        start_core(core, &slave_core_main);
        while(!finished_core) {
            asm volatile("nop");
        }
        uart_write_newline();
    }

    uart_write_string("Back at main core!\n");

    init_el2_vectors();
    el2_exception_handlers[EXCEPTION_SYNC_EL1_64] = handle_sync_el1_64;  // set handler for hvc instruction
    step_down_to_el1();
}
