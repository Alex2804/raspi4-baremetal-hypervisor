#include "utils/uart.h"
#include "utils/string.h"
#include "utils/multicore.h"

volatile int finished_core = 0;
char* memory_base_address = 0;

void slave_core_main() {
    uart_write_string("Starting program at address ");
    uart_write_hex((unsigned long long) memory_base_address);
    uart_write_string(" from core ");
    uart_write_long(get_core_id());
    uart_write_string("...\n");

    int (*function_pointer)(int, int, int) = (int (*)(int, int, int)) memory_base_address;
    int result = function_pointer(get_core_id() * 5, 34, 12);  // (core_id * 5) + 34 - 12 + 42 = 69/74/79
    uart_write_string("Result from core ");
    uart_write_long(get_core_id());
    uart_write_string(": ");
    uart_write_long(result);
    uart_write_newline();

    finished_core = 1;
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

    int count = 0;
    while(count <= 0) {
        uart_write_string("Please type in the number of bytes of the program:\n");
        uart_read_until_char(memory_base_address, 512, '\n');
        count = string_to_int(memory_base_address);
    }

    uart_write_string("\nPlease type in the chars of the program:\n");
    for(int i = 0; i < count; ++i) {
        memory_base_address[i] = uart_read_char();
        uart_write_char(memory_base_address[i]);
    }
    uart_write_string("\nSaved the program to memory.\n\n");

    for (int core = 1; core < 4; ++core) {
        finished_core = 0;
        start_core(core, &slave_core_main);
        while(!finished_core) {
            asm volatile("nop");
        }
        uart_write_newline();
    }

    slave_core_main();

    while (1) {
        const char c = uart_read_char();
        if(c == '\n') {
            uart_write_newline();
        } else {
            uart_write_char(c);
        }
    }
}
