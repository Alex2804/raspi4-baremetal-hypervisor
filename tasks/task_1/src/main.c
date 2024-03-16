#include "utils/uart.h"
#include "utils/exceptions/exceptions.h"
#include "utils/exceptions/el2.h"

void step_downward_to_el0_code() {
    uart_write_string("We are now at EL0\n");

    while(1) {
        char c = uart_read_char();
        uart_write_char(c);
        uart_write_char(c);
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
        const spsr_elx_t spsr = spsr_elx_create_for_el_change(1, 0, 0);
        el_downstep(el, spsr, &step_downward_to_el1_code); // return to aarch64 el1 with sp_el0
    }
}

void main()
{
    uart_init();
    init_el2_vectors();

    print_el("Initialy we are at EL");

    step_down_to_el1();
    while(1) {
        char c = uart_read_char();
        uart_write_char(c);
        uart_write_char(c);
    }
}
