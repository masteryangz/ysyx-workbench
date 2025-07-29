#include <sim_main.h>

VerilatedContext* contextp = nullptr;
VerilatedFstC* tfp = nullptr;
//VTop* top;

void init_sdb();
void sdb_mainloop();

const uint64_t max_cycles = 100000;
uint64_t sim_time = 0;

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
  init_sdb();
  sdb_mainloop();
  //const uint64_t max_cycles = 100000;
  //uint64_t sim_time = 0;
/*
  while (sim_time < max_cycles && !contextp->gotFinish()) {
    printf("Sim time: %lu\n", sim_time);
    top->clock = 0; step_and_dump_wave();
    top->clock = 1; step_and_dump_wave();
    sim_time++;
  }
*/
  printf("sim_main finished with %s trap\n", top->io_goodTrap ? "good" : "bad");
  sim_exit();
  delete top;
  delete tfp;
  delete contextp;
  return 0;
}
