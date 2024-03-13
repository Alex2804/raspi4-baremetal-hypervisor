#include "utils/multicore.h"

extern unsigned long long spin_core_0;

void start_core(unsigned int core_number, void (*func)(void))
{
  if(core_number > 3)
    return;
  *(&spin_core_0 + core_number) = (unsigned long long) func;
  asm volatile("sev");
}

void clear_core(unsigned int core_number)
{
  if(core_number > 3)
    return;
  *(&spin_core_0 + core_number) = 0;
}
