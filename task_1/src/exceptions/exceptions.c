#include "exceptions/exceptions.h"

#include "uart.h"

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address) {
    uart_puts("Exception type: ");
    uart_long(type);
    uart_puts("; return address: ");
    uart_hex(address);
    uart_puts("; ESR: ");
    uart_hex(esr);
    uart_puts(")\n");

    volatile int i = 0;
    while(i == 0) {}
}
