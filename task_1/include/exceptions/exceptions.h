#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#ifndef __ASSEMBLER__

int get_el();

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address);

#endif

#endif //EXCEPTIONS_H
