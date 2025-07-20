#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/Vdouble_switch.h"

VerilatedContext* contextp = NULL;
VerilatedFstC* tfp = NULL;

static Vdouble_switch* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedFstC;
  top = new Vdouble_switch;
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

    top->a=0; top->b=0;  step_and_dump_wave();
                top->b=1;  step_and_dump_wave();
    top->a=1; top->b=0;  step_and_dump_wave();
                top->b=1;  step_and_dump_wave();

  sim_exit();
}