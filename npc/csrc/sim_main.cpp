#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/VTop.h"

VerilatedContext* contextp = nullptr;
VerilatedFstC* tfp = nullptr;
static VTop* top;

void step_and_dump_wave() {
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}

void sim_init() {
  contextp = new VerilatedContext;
  tfp = new VerilatedFstC;
  top = new VTop;

  contextp->traceEverOn(true);
  top->trace(tfp, 99);  // 99 is standard depth
  tfp->open("dump.fst");
}

void sim_exit() {
  tfp->close();
}

int main() {
  sim_init();

  const uint64_t max_cycles = 100000;
  uint64_t sim_time = 0;

  top->reset = 1;
  for (int i = 0; i < 2; ++i) {
    top->clock = 0; step_and_dump_wave();
    top->clock = 1; step_and_dump_wave();
  }
  top->reset = 0;

  while (sim_time < max_cycles && !contextp->gotFinish()) {
    printf("Sim time: %lu\n", sim_time);
    top->clock = 0; step_and_dump_wave();
    top->clock = 1; step_and_dump_wave();
    sim_time++;
  }
  printf("sim_main finished");
  sim_exit();
  delete top;
  delete tfp;
  delete contextp;
  return 0;
}
