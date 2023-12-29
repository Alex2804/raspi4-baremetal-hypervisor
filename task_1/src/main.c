#include "uart.h"
#include "exceptions/exceptions.h"
#include "exceptions/el2.h"

void step_up_to_el2() {
    uart_puts("Before hvc instruction, we are at EL");
    uart_long(get_el());
    uart_send('\n');

    asm("hvc #0xf2");

    uart_puts("After hvc instruction, we are at EL");
    uart_long(get_el());
    uart_send('\n');
}

void step_down_to_el0();
void step_down_to_el1();

void step_downward_to_el0_code() {
    uart_puts("EL0\n");
    while(1) {
        char c = uart_getc();
        uart_send(c);
        uart_send(c);
    }
}
void step_downward_to_el1_code() {
    uart_puts("We are now at EL");
    uart_long(get_el());
    uart_send('\n');

    //step_down_to_el0();
    // step up to el2 with exception
    step_up_to_el2();
    while(1) {
        char c = uart_getc();
        uart_send(c);
        uart_send(c);
    }
}

void el_downstep(int current_el, void* eret_addr, long spsr_bitmask) {
    uart_puts("stepping down from EL");
    uart_long(current_el);
    uart_puts(" to EL");
    uart_long(current_el - 1);
    uart_puts("\n");

    if(current_el == 3) {
        asm("msr elr_el3, %0\n\t"
            "msr spsr_el3, %1\n\t"
            "eret": : "r"(eret_addr), "r"(spsr_bitmask));
    } else if(current_el == 2) {
        asm("msr elr_el2, %0\n\t"
            "msr spsr_el2, %1\n\t"
            "eret" : : "r"(eret_addr), "r"(spsr_bitmask));
    } else if(current_el == 1) {
        asm("msr elr_el1, %0\n\t"
            "msr spsr_el1, %1\n\t"
            "eret" : : "r"(eret_addr), "r"(spsr_bitmask));
    }
}
void step_down_to_el0() {
    const int el = get_el();
    el_downstep(el, &step_downward_to_el0_code, 0b00000); // return to aarch64 el0 with (implicit) sp_el0
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
