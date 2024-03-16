#include "utils/uart.h"
#include "utils/multicore.h"
#include "utils/exceptions/el2.h"
#include "utils/exceptions/exceptions.h"


void step_downward_to_el0_code() {
    uart_write_string("We are now at EL0\n");

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
    step_down_to_el1();
}

void main()
{
    uart_init();
    uart_write_string("Hello from main core (");
    uart_write_long(get_core_id());
    uart_write_string(")!\n");

    extern unsigned long long __spin_core_0;
    for(unsigned long long i = 0; i < 4; ++i) {
        uart_write_string("Address for core ");
        uart_write_long(i);
        uart_write_string(" is ");
        uart_write_hex((unsigned long long)(&__spin_core_0 + i));

        uart_write_string(" with value ");
        uart_write_hex(*(&__spin_core_0 + i));
        uart_write_newline();
    }

    uart_write_string("Starting other cores ...\n");

    for(int core = 1; core < 4; ++core) {
        start_core(core, &slave_core_main);
        for(unsigned long long i = 0; i < 100000; ++i) {
            asm volatile("nop");
        }
        uart_write_string("type 'c' to continue\n");
        while(uart_read_char() != 'c') {
            asm volatile("nop");
        }
    }

    uart_write_string("Back at main core!\n");

    init_el2_vectors();
    step_down_to_el1();
}
