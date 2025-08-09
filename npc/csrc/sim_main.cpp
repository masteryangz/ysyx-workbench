#include <sim_main.h>

VerilatedContext* contextp = nullptr;
VerilatedFstC* tfp = nullptr;
VTop* top;

//extern "C" void set_pc_ptr(uint32_t ptr);
//extern "C" void set_gpr_ptr();
//extern "C" void set_instr_mem_ptr(uint64_t ptr);

void init_sdb();
void sdb_mainloop();

const uint64_t max_cycles = 100000;
uint64_t sim_time = 0;

//extern long long int get_sv_time();

void step_and_dump_wave() {  
  //Log("Sim time: %lu", sim_time);
  top->eval();
  //Log("Sim time: %lu", sim_time);
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
  ringbuf_init(&iringbuf);
}

void sim_exit() {
  tfp->close();
}

static void welcome() {
  Log("Trace: %s", MUXDEF(CONFIG_TRACE, ANSI_FMT("ON", ANSI_FG_GREEN), ANSI_FMT("OFF", ANSI_FG_RED)));
  IFDEF(CONFIG_TRACE, Log("If trace is enabled, a log file will be generated "
        "to record the trace. This may lead to a large log file. "
        "If it is not necessary, you can disable it in menuconfig"));
  Log("Build time: %s, %s", __TIME__, __DATE__);
  printf("Welcome to %s-npc!\n", ANSI_FMT(str(__GUEST_ISA__), ANSI_FG_YELLOW ANSI_BG_RED));
  printf("For help, type \"help\"\n");
  // Log("Exercise: Please remove me in the source code and compile NEMU again.");
  // assert(0);
}

int main() {
  //Log("Starting simulation...\n");
  sim_init();
  //Log("Simulation initialized.\n");
  init_mem_from_file("rom.txt");
  //Log("Memory initialized from rom.txt.\n");
  init_sdb();
  //Log("SDB initialized.\n");
  welcome();
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
