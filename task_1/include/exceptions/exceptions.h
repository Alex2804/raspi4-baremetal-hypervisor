#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#ifndef __ASSEMBLER__

int get_el();
void el_downstep(int current_el, void* eret_addr, long spsr_bitmask);

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address);

#endif

#endif //EXCEPTIONS_H
