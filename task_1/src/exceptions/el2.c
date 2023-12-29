#include "exceptions/el2.h"

#include "exceptions/exceptions.h"
#include "uart.h"

void handle_sync_el1_64(unsigned long esr, unsigned long address) {
    if((esr & ESR_EL2_EC) == ESR_EL2_EC_HVC_AARCH64) {
        uart_puts("hvc from EL1, we are now at EL");
        uart_long(get_el());
        uart_send('\n');
    } else {
        show_invalid_entry_message(SYNC_INVALID_EL1_64, esr, address);
    }
}
