#include <stdio.h>
#include <stdlib.h>
#include <svdpi.h>
#include <sim_main.h>
#include "verilated.h"

extern "C" void sim_end(int pc) {
  printf("[sim_end] Called: marking finish.\n");
  nemu_state.halt_pc = pc;
  Verilated::gotFinish(true);
}
