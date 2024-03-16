#ifndef MULTICORE_H
#define MULTICORE_H

int get_core_id();

void start_core(unsigned long long core_number, void (*func)(void));
void clear_core(unsigned long long core_number);  // not necessary, core is cleared automatically by assembly in start.S

#endif //MULTICORE_H
