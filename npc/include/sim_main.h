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

void step_and_dump_wave();
void sim_init();
void sim_exit();

#endif