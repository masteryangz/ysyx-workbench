#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/VTop.h"
#include <sdb/sdb.h>

extern VerilatedContext* contextp;
extern VerilatedFstC* tfp;
static VTop* top;
extern const uint64_t max_cycles;
extern uint64_t sim_time;

void step_and_dump_wave();
void sim_init();
void sim_exit();