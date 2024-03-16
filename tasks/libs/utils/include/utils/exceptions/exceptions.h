#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "utils/registers/spsr_elx.h"

int get_el();
void print_el(const char* message);

void el_downstep(int current_el, spsr_elx_t spsr, void* eret_addr);

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address);

#endif //EXCEPTIONS_H
