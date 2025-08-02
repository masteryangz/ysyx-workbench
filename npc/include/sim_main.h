#ifndef SIM_MAIN_H
#define SIM_MAIN_H

#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/VTop.h"
#include <sdb/sdb.h>

extern VerilatedContext* contextp;
extern VerilatedFstC* tfp;
extern VTop* top;
extern const uint64_t max_cycles;
extern uint64_t sim_time;
extern vaddr_t mem_read_addr;
extern word_t mem_read_data;
extern word_t gpr_x0;
extern word_t gpr_x1;
extern word_t gpr_x2;
extern word_t gpr_x3;
extern word_t gpr_x4;
extern word_t gpr_x5;
extern word_t gpr_x6 ;
extern word_t gpr_x7;
extern word_t gpr_x8;
extern word_t gpr_x9;
extern word_t gpr_x10;
extern word_t gpr_x11;
extern word_t gpr_x12;
extern word_t gpr_x13;
extern word_t gpr_x14;
extern word_t gpr_x15;
extern word_t gpr_x16;
extern word_t gpr_x17;
extern word_t gpr_x18;
extern word_t gpr_x19;
extern word_t gpr_x20;
extern word_t gpr_x21;
extern word_t gpr_x22;
extern word_t gpr_x23;
extern word_t gpr_x24;
extern word_t gpr_x25;
extern word_t gpr_x26;
extern word_t gpr_x27;
extern word_t gpr_x28;
extern word_t gpr_x29;
extern word_t gpr_x30;
extern word_t gpr_x31;
extern word_t pc_ptr;

void step_and_dump_wave();
void sim_init();
void sim_exit();

#endif