#include "utils/uart.h"
#include "utils/string.h"

void main()
{
    uart_init();

    unsigned long size = 0;
    char* memory_base_address;

    asm volatile("ldr %0, = _start" : "=r" (memory_base_address));
    memory_base_address += 2 << 20;  // add 1 MB to the base address to get the address of the first "free" memory location
    uart_write_string("Memory base address: ");
    uart_write_hex((unsigned long long) memory_base_address);
    uart_write_newline();

    int count = 0;
    while(count <= 0) {
        uart_write_string("Please type in the number of chars you would like to type in until echoed back.\n");
        uart_read_until_char(memory_base_address, 512, '\n');
        count = string_to_int(memory_base_address);
    }

    uart_write_string("Please type, it will be echoed back after the previous defined count.\n");

    int typed_in = 0;
    while(1) {
        char c = uart_read_char();
        memory_base_address[typed_in++] = c;
        if(typed_in >= count) {
            memory_base_address[typed_in] = '\0';
            uart_write_string(memory_base_address);
            uart_write_newline();
            typed_in = 0;
        }
    }
}
