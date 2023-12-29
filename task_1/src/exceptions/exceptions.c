#include "exceptions/exceptions.h"

#include "uart.h"

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address) {
    uart_put_string("Exception type: ");
    uart_put_long(type);
    uart_put_string("; return address: ");
    uart_put_hex(address);
    uart_put_string("; ESR: ");
    uart_put_hex(esr);
    uart_put_string(")\n");

    volatile int i = 0;
    while(i == 0) {}
}
