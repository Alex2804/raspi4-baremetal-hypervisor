#include "uart.h"
#include "registers.h"
#include "exceptions/exceptions.h"
#include "exceptions/el2.h"


void step_downward_to_el0_code() {
    uart_puts("We are now at EL0\n");

    PRINT_ALL_REGISTERS(0);

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

    PRINT_ALL_REGISTERS(get_el());
    step_up_to_el2();

    PRINT_ALL_REGISTERS(get_el());
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
    init_el2_vectors();
    uart_init();

    // enable fp and simd instructions at EL0 and EL1
    unsigned int tmp = (0x3 << 20);
    asm volatile("msr cpacr_el1, %0" : : "r" (tmp)); // enable fp and simd instructions

    uart_puts("Initialy we are at EL");
    uart_long(get_el());
    uart_send('\n');

    asm volatile("mov x0, #1");
    asm volatile("mov v3.2D[0], x0");
    asm volatile("mov x0, #2");
    asm volatile("mov v3.2D[1], x0");

    PRINT_ALL_REGISTERS(get_el());

    step_down_to_el1();
}
