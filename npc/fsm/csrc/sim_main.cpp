#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/Vfsm.h"

VerilatedContext* contextp = NULL;
VerilatedFstC* tfp = NULL;

static Vfsm* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedFstC;
  top = new Vfsm;
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

  sim_exit();
}