#include <stdio.h>
#include <stdlib.h>
#include <svdpi.h>
#include "verilated.h"

uint32_t *gpr_array = NULL;
uint32_t *pc_ptr = NULL;
uint32_t *instr_mem_ptr = NULL;

extern "C" void sim_end() {
  printf("[sim_end] Called: marking finish.\n");
  Verilated::gotFinish(true);
}

extern "C" void set_gpr_ptr(uint64_t ptr) {
  gpr_array = (uint32_t *)ptr;
}

extern "C" void set_pc_ptr(uint64_t ptr) {
  pc_ptr = (uint32_t *)ptr;
}

void set_instr_mem_ptr(uint64_t ptr) {
  instr_mem_ptr = (uint32_t *)ptr;
}