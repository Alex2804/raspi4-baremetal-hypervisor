#include <utils/exceptions/el2.h>
#include <utils/exceptions/exceptions.h>
#include <utils/registers/hcr_el2.h>

#include "utils/uart.h"
#include "utils/string.h"
#include "utils/multicore.h"

volatile int task_finished = 0;

extern void _start();

void handle_any_exception() {
    uart_write_string("Error on program execution - ELR: ");
    uart_write_hex(get_elr_el2());
    uart_write_string("; ESR: ");
    uart_write_hex(get_esr_el2());
    uart_write_newline();

    task_finished = 1;
    asm volatile("sev");

    // return back to _start to wait for new program to execute
    auto spsr = spsr_elx_create_for_el_change(2, 0, 1);
    const hcr_el2_t hcr_el2 = hcr_el2_create_from_spsr(spsr);
    asm volatile("msr hcr_el2, %0\n\t"
                 "msr elr_el2, %1\n\t"
                 "msr spsr_el2, %2\n\t"
                 "eret" : : "r"(hcr_el2), "r"(_start), "r"(spsr));
}

char* memory_base_address = 0;

void slave_core_main() {
    init_el2_vectors();
    for(int i = 0; i < 16; ++i) {
        el2_exception_handlers[i] = handle_any_exception;
    }

    uart_write_string("Starting program at address ");
    uart_write_hex((unsigned long long) memory_base_address);
    uart_write_string(" from core ");
    uart_write_long(get_core_id());
    uart_write_string("...\n");

    int (*function_pointer)(int, int, int) = (int (*)(int, int, int)) memory_base_address;
    int result = function_pointer(get_core_id() * 5, 34, 12);  // (core_id * 5) + 34 - 12 + 42 = 64/69/74/79
    uart_write_string("Result from core ");
    uart_write_long(get_core_id());
    uart_write_string(": ");
    uart_write_long(result);
    uart_write_newline();

    task_finished = 1;
    asm volatile("sev");
}

void main() {
    uart_init();

    uart_write_string("Please type in 's' to start.\n");
    while(uart_read_char() != 's') {}

    uart_write_string("Hello from main core!\n");

    asm volatile("ldr %0, = _start" : "=r" (memory_base_address));
    memory_base_address += 2 << 20;  // add 1 MB to the base address to get the address of the first "free" memory location
    uart_write_string("Memory base address: ");
    uart_write_hex((unsigned long long) memory_base_address);
    uart_write_newline();
    uart_write_newline();

    char c = ' ';
    while(c != 'e') {
        if(c == 'c') {
            int count = 0;
            while(count <= 0) {
                uart_write_string("Please type in the number of bytes of the program:\n");
                uart_read_until_char(memory_base_address, 512, '\n');
                count = string_to_int(memory_base_address);
            }

            uart_write_string("Please type in the chars of the program:\n");
            for(int i = 0; i < count; ++i) {
                memory_base_address[i] = uart_read_char();
                uart_write_char(memory_base_address[i]);
            }
            uart_write_string("\nSaved the program to memory.\n");

            task_finished = 0;
            start_core(1, &slave_core_main);

            while(!task_finished) {
                asm volatile("wfe");
            }
            uart_write_newline();
        }
        uart_write_string("type 'c' to continue and 'e' to exit\n");
        c = uart_read_char();
    }

    while (1) {
        c = uart_read_char();
        if(c == '\n') {
            uart_write_newline();
        } else {
            uart_write_char(c);
        }
    }
}
