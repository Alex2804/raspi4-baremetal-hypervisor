#ifndef MULTICORE_H
#define MULTICORE_H

void start_core(unsigned int core_number, void (*func)(void));
void clear_core(unsigned int core_number);

#endif //MULTICORE_H
