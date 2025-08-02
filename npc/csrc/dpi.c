#include <stdio.h>
#include <stdlib.h>
#include <svdpi.h>
#include <sim_main.h>
#include "verilated.h"

//extern uint32_t *gpr_array = NULL;
word_t gpr_x0 = 0;
word_t gpr_x1 = 0;
word_t gpr_x2 = 0;
word_t gpr_x3 = 0;
word_t gpr_x4 = 0;
word_t gpr_x5 = 0;
word_t gpr_x6 = 0;
word_t gpr_x7 = 0;
word_t gpr_x8 = 0;
word_t gpr_x9 = 0;
word_t gpr_x10 = 0;
word_t gpr_x11 = 0;
word_t gpr_x12 = 0;
word_t gpr_x13 = 0;
word_t gpr_x14 = 0;
word_t gpr_x15 = 0;
word_t gpr_x16 = 0;
word_t gpr_x17 = 0;
word_t gpr_x18 = 0;
word_t gpr_x19 = 0;
word_t gpr_x20 = 0;
word_t gpr_x21 = 0;
word_t gpr_x22 = 0;
word_t gpr_x23 = 0;
word_t gpr_x24 = 0;
word_t gpr_x25 = 0;
word_t gpr_x26 = 0;
word_t gpr_x27 = 0;
word_t gpr_x28 = 0;
word_t gpr_x29 = 0;
word_t gpr_x30 = 0;
word_t gpr_x31 = 0;
word_t pc_ptr = 0;
//extern uint32_t *instr_mem_ptr = NULL;

extern "C" void sim_end() {
  printf("[sim_end] Called: marking finish.\n");
  Verilated::gotFinish(true);
}
/*
extern "C" void set_gpr_ptr(uint64_t ptr) {
  gpr_array = (uint32_t *)ptr;
}
*/
extern "C" void set_gpr_ptr() {
  gpr_x0 = top->io_rf_out_0;
  gpr_x1 = top->io_rf_out_1;
  gpr_x2 = top->io_rf_out_2;
  gpr_x3 = top->io_rf_out_3;
  gpr_x4 = top->io_rf_out_4;
  gpr_x5 = top->io_rf_out_5;
  gpr_x6 = top->io_rf_out_6;
  gpr_x7 = top->io_rf_out_7;
  gpr_x8 = top->io_rf_out_8;
  gpr_x9 = top->io_rf_out_9;
  gpr_x10 = top->io_rf_out_10;
  gpr_x11 = top->io_rf_out_11;
  gpr_x12 = top->io_rf_out_12;
  gpr_x13 = top->io_rf_out_13;
  gpr_x14 = top->io_rf_out_14;
  gpr_x15 = top->io_rf_out_15;
  gpr_x16 = top->io_rf_out_16;
  gpr_x17 = top->io_rf_out_17;
  gpr_x18 = top->io_rf_out_18;
  gpr_x19 = top->io_rf_out_19;
  gpr_x20 = top->io_rf_out_20;
  gpr_x21 = top->io_rf_out_21;
  gpr_x22 = top->io_rf_out_22;
  gpr_x23 = top->io_rf_out_23;
  gpr_x24 = top->io_rf_out_24;
  gpr_x25 = top->io_rf_out_25;
  gpr_x26 = top->io_rf_out_26;
  gpr_x27 = top->io_rf_out_27;
  gpr_x28 = top->io_rf_out_28;
  gpr_x29 = top->io_rf_out_29;
  gpr_x30 = top->io_rf_out_30;
  gpr_x31 = top->io_rf_out_31;
}

extern "C" void set_pc_ptr(uint32_t ptr) {
  pc_ptr = ptr;
  Log("PC pointer set to %08x\n", pc_ptr);
}
/*
extern "C" void set_instr_mem_ptr(uint64_t ptr) {
  instr_mem_ptr = (uint32_t *)ptr;
}
*/