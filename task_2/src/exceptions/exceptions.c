#include "exceptions/exceptions.h"

#include "uart.h"

void el_downstep(int current_el, void* eret_addr, long spsr_bitmask) {
    uart_puts("stepping down from EL");
    uart_long(current_el);
    uart_puts(" to EL");
    uart_long(current_el - 1);
    uart_puts("\n");

    if(current_el == 3) {
        asm("msr elr_el3, %0\n\t"
            "msr spsr_el3, %1\n\t"
            "eret": : "r"(eret_addr), "r"(spsr_bitmask));
    } else if(current_el == 2) {
        asm("msr elr_el2, %0\n\t"
            "msr spsr_el2, %1\n\t"
            "eret" : : "r"(eret_addr), "r"(spsr_bitmask));
    } else if(current_el == 1) {
        asm("msr elr_el1, %0\n\t"
            "msr spsr_el1, %1\n\t"
            "eret" : : "r"(eret_addr), "r"(spsr_bitmask));
    }
}

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
