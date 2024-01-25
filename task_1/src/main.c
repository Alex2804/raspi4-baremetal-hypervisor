#include "uart.h"
#include "exceptions/exceptions.h"
#include "exceptions/el2.h"


void step_downward_to_el0_code() {
    uart_puts("We are now at EL0\n");
    while(1) {
        char c = uart_getc();
        uart_send(c);
        uart_send(c);
    }
}
void step_down_to_el0() {
    el_downstep(get_el(), &step_downward_to_el0_code, 0b00000); // return to aarch64 el0 with (implicit) sp_el0
}


void step_up_to_el2() {
    uart_puts("Before hvc instruction, we are at EL");
    uart_long(get_el());
    uart_send('\n');

    asm("hvc #0xf2");

    uart_puts("After hvc instruction, we are at EL");
    uart_long(get_el());
    uart_send('\n');
}


void step_downward_to_el1_code() {
    uart_puts("We are now at EL");
    uart_long(get_el());
    uart_send('\n');

    step_up_to_el2();
    step_down_to_el0();
}
void step_down_to_el1() {
    const int el = get_el();
    if(el == 3 || el == 2) {
        el_downstep(el, &step_downward_to_el1_code, 0b00100); // return to aarch64 el1 with sp_el0
    }
}

void main()
{
    uart_init();
    init_el2_vectors();

    uart_puts("Initialy we are at EL");
    uart_long(get_el());
    uart_send('\n');

    step_down_to_el1();
}
