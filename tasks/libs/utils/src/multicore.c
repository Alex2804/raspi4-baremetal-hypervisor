#include "utils/multicore.h"

#include <utils/uart.h>

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

/*
Hello from main core (0)!
Starting other cores ...
starting core: 1
address for core 0 is 0x800D8 with value 0x0
address for core 1 is 0x800E0 with value 0x80254
address for core 2 is 0x800E8 with value 0x0
address for core 3 is 0x800F0 with value 0x0
starting core: 2
address for core 0 is 0x800D8 with value 0x0
address for core 1 is 0x800E0 with value 0x80254
address for core 2 is 0x800E8 with value 0x80254
address for core 3 is 0x800F0 with value 0x0
Hello from core 1!
Hello from core 2!
starting core: 3
address for core 0 is 0x800D8 with value 0x0
address for core 1 is 0x800E0 with value 0x0
address for core 2 is 0x800E8 with value 0x0
address for core 3 is 0x800F0 with value 0x80254
Hello from core 3!
Back at main core!
*/
