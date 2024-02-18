#include "uart.h"

void main()
{
    uart_init();
    uart_write_char('b');
    uart_write_newline();
    uart_write_char('c');
    uart_write_newline();
    uart_write_string("Hello world!\n");
    uart_write_hex(0x123456789ABCDEF);
    uart_write_newline();
    uart_write_long(123456789);
    uart_write_newline();
    while (1);
}
