#include <am.h>
#include <klib-macros.h>
#include "npc.h"


Area heap = RANGE(&_heap_start, PMEM_END);
static const char mainargs[MAINARGS_MAX_LEN] = TOSTRING(MAINARGS_PLACEHOLDER); // defined in CFLAGS

void putch(char ch) {
  outb(SERIAL_PORT, ch);
}

void halt(int code) {
  register int a0 asm("a0") = code;  // place exit code in a0
  asm volatile("ebreak" : : "r"(a0)); // trigger trap with code in a0
  while (1);
}

void _trm_init() {
  int ret = main(mainargs);
  halt(ret);
}
