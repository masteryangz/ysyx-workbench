#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/VTop.h"

VerilatedContext* contextp = NULL;
VerilatedFstC* tfp = NULL;

static VTop* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedFstC;
  top = new VTop;
  contextp->traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open("dump.fst");
}

void sim_exit(){
  step_and_dump_wave();
  tfp->close();
}

int main() {
  sim_init();
  top->reset = 1;
  top->clock = 0;
  // reset logic
  for (int i = 0; i < 2; ++i) {
    step_and_dump_wave();
    top->clock = !top->clock;
  }
  top->reset = 0;
  // read rom
  for (int i = 0; i < 5; i++) {
    step_and_dump_wave();
    top->clock = !top->clock;
    step_and_dump_wave();
    top->clock = !top->clock;
    
  }

  sim_exit();
}