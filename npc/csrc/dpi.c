#include <stdio.h>
#include <stdlib.h>
#include <svdpi.h>
#include <sim_main.h>
#include "verilated.h"

extern "C" void sim_end() {
  printf("[sim_end] Called: marking finish.\n");
  Verilated::gotFinish(true);
}
