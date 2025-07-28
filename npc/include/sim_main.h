#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/VTop.h"

extern VerilatedContext* contextp;
extern VerilatedFstC* tfp;
static VTop* top;
const uint64_t max_cycles = 100000;
uint64_t sim_time = 0;

void step_and_dump_wave();
void sim_init();
void sim_exit();