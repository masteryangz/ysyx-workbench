#include <sim_main.h>

VerilatedContext* contextp = nullptr;
VerilatedFstC* tfp = nullptr;
VTop* top;

void engine_start();
void init_monitor(int, char *[]);
int is_exit_status_bad();

const uint64_t max_cycles = 1000000;
uint64_t sim_time = 0;

//extern long long int get_sv_time();

void step_and_dump_wave() {  
  //Log("Sim time: %lu", sim_time);
  top->eval();
  //Log("hi");
  contextp->timeInc(1);
  //Log("Sim time: %lu", sim_time);
  tfp->dump(contextp->time());
}

void sim_init() {
  contextp = new VerilatedContext;
  tfp = new VerilatedFstC;
  top = new VTop;
  contextp->traceEverOn(true);
  top->trace(tfp, 99);  // 99 is standard depth
  tfp->open("dump.fst");
  ringbuf_init(&iringbuf);
}

void sim_exit() {
  tfp->close();
}

int main(int argc, char *argv[]) {
  //Log("Starting simulation...\n");
  TODO("replace when and switch with Mux1H");
  sim_init();
  //Log("Simulation initialized.\n");
  init_monitor(argc, argv);
  engine_start();
/*
  while (sim_time < max_cycles && !contextp->gotFinish()) {
    printf("Sim time: %lu\n", sim_time);
    top->clock = 0; step_and_dump_wave();
    top->clock = 1; step_and_dump_wave();
    sim_time++;
  }
*/
  //printf("sim_main finished with %s trap\n", top->io_goodTrap ? "good" : "bad");
  sim_exit();
  delete top;
  delete tfp;
  delete contextp;
  return is_exit_status_bad();
}
