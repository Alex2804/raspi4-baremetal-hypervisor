#include "utils/multicore.h"

extern unsigned long long __spin_core_0;

void start_core(unsigned long long core_number, void (*func)(void))
{
  if(core_number > 3)
    return;

  *(&__spin_core_0 + core_number) = (unsigned long long) func;

  while(*(&__spin_core_0 + core_number) != 0) {
    asm volatile("sev");
  }
}

void clear_core(unsigned long long core_number)
{
  if(core_number > 3)
    return;
  *(&__spin_core_0 + core_number) = 0;
}
