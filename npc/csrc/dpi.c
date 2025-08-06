#include <stdio.h>
#include <stdlib.h>
#include <svdpi.h>
#include <sim_main.h>
#include "verilated.h"

//extern uint32_t *gpr_array = NULL;
/*
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
*/
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
/*
extern "C" void set_gpr_ptr() {
  cpu.gpr[0] = top->io_rf_out_0;
  cpu.gpr[1] = top->io_rf_out_1;
  cpu.gpr[2] = top->io_rf_out_2;
  cpu.gpr[3] = top->io_rf_out_3;
  cpu.gpr[4] = top->io_rf_out_4;
  cpu.gpr[5] = top->io_rf_out_5;
  cpu.gpr[6] = top->io_rf_out_6;
  cpu.gpr[7] = top->io_rf_out_7;
  cpu.gpr[8] = top->io_rf_out_8;
  cpu.gpr[9] = top->io_rf_out_9;
  cpu.gpr[10] = top->io_rf_out_10;
  cpu.gpr[11] = top->io_rf_out_11;
  cpu.gpr[12] = top->io_rf_out_12;
  cpu.gpr[13] = top->io_rf_out_13;
  cpu.gpr[14] = top->io_rf_out_14;
  cpu.gpr[15] = top->io_rf_out_15;
  cpu.gpr[16] = top->io_rf_out_16;
  cpu.gpr[17] = top->io_rf_out_17;
  cpu.gpr[18] = top->io_rf_out_18;
  cpu.gpr[19] = top->io_rf_out_19;
  cpu.gpr[20] = top->io_rf_out_20;
  cpu.gpr[21] = top->io_rf_out_21;
  cpu.gpr[22] = top->io_rf_out_22;
  cpu.gpr[23] = top->io_rf_out_23;
  cpu.gpr[24] = top->io_rf_out_24;
  cpu.gpr[25] = top->io_rf_out_25;
  cpu.gpr[26] = top->io_rf_out_26;
  cpu.gpr[27] = top->io_rf_out_27;
  cpu.gpr[28] = top->io_rf_out_28;
  cpu.gpr[29] = top->io_rf_out_29;
  cpu.gpr[30] = top->io_rf_out_30;
  cpu.gpr[31] = top->io_rf_out_31;
}

extern "C" void set_pc_ptr(uint32_t ptr) {
  cpu.pc = ptr;
  //Log("PC pointer set to %08x\n", pc_ptr);
}
*/
/*
extern "C" void set_instr_mem_ptr(uint64_t ptr) {
  instr_mem_ptr = (uint32_t *)ptr;
}
*/