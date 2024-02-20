#include "exceptions/el2.h"

#include "exceptions/exceptions.h"

#include "utils/uart.h"

void handle_sync_el1_64(unsigned long esr, unsigned long address) {
    if((esr & ESR_EL2_EC) == ESR_EL2_EC_HVC_AARCH64) {
        uart_write_string("hvc from EL1, we are now at EL");
        uart_write_long(get_el());
        uart_write_newline();
    } else {
        uart_write_string("handle_sync_el1_64 called without known esr -> ");
        show_invalid_entry_message(SYNC_INVALID_EL1_64, esr, address);
    }
}
