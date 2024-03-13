#include "utils/uart.h"
#include "utils/multicore.h"
#include "utils/exceptions/el2.h"
#include "utils/exceptions/exceptions.h"


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
        const spsr_elx_t spsr = spsr_elx_create_for_el_change(1, 0, 1);
        el_downstep(el, spsr, &step_downward_to_el1_code); // return to aarch64 el1 with sp_el0
    }
}

void slave_core_main() {
    int core_id = -1;
    asm volatile("mrs %0, mpidr_el1\n\t"
                 "and %0, %0, 0x3" : "=r" (core_id));
    clear_core(core_id);

    uart_write_string("Hello from core ");
    uart_write_long(core_id);
    uart_write_string("!\n");

    init_el2_vectors();
    step_down_to_el1();
}

void main()
{
    uart_init();

    for(int core = 1; core < 4; ++core) {
        start_core(core, &slave_core_main);
        for(unsigned long long i = 0; i < 100000000; ++i) {
            asm volatile("nop");
        }
    }

    uart_write_string("Hello from main core!\n");
    print_el("Initialy we are at EL");
    init_el2_vectors();
    step_down_to_el1();
}
