#include "utils/uart.h"

#include "task4_program.h"

void main()
{
    uart_init();

    char* memory_base_address;

    asm volatile("ldr %0, = _start" : "=r" (memory_base_address));
    memory_base_address += (2 << 20);  // add 1 MB to the base address to get the address of the first "free" memory location
    uart_write_string("Memory base address: ");
    uart_write_hex((unsigned long long) memory_base_address);
    uart_write_newline();

    for(int i = 0; i < program_len; i++) {
        memory_base_address[i] = program[i];
    }
    uart_write_string("Saved the chars to memory.\n");

    int (*function_pointer)(int, int, int) = (int (*)(int, int, int)) memory_base_address;
    int result = function_pointer(5, 34, 12);
    uart_write_string("Result: ");
    uart_write_long(result);
    uart_write_newline();

    while(1) {
        char c = uart_read_char();
        uart_write_char(c);
    }
}
