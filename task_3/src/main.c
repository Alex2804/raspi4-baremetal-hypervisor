#include "utils/uart.h"

int string_to_int(const char* string) {
    int value = 0;
    int sign = 1;
    if(*string == '-') {
        sign = -1;
        ++string;
    }
    for(char c = *string; c != '\0'; c = *++string) {
        if(c < '0' || c > '9') {
            return 0;
        }
        value *= 10;
        value += c - '0';
    }
    value *= sign;
    return value;
}

int read_until_char(char* buffer, char stop_char) {
    int read_chars = 0;
    while(1) {
        char c = uart_read_char();
        if(c == '\n') {
            uart_write_newline();
        } else {
            uart_write_char(c);
        }
        if(c == stop_char) {
            *buffer = '\0';
            return read_chars;
        }
        *buffer = c;
        ++buffer;
        ++read_chars;
    }
}

void main()
{
    uart_init();

    unsigned long size = 0;
    char* memory_base_address;

    asm volatile("ldr %0, = _start" : "=r" (memory_base_address));
    memory_base_address += (2 << 20);  // add 1 MB to the base address to get the address of the first "free" memory location
    uart_write_string("Memory base address: ");
    uart_write_hex((unsigned long long) memory_base_address);
    uart_write_newline();

    int count = 0;
    while(count <= 0) {
        uart_write_string("Please type in the number of chars you would like to type in until echoed back.\n");
        read_until_char(memory_base_address, '\n');
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
