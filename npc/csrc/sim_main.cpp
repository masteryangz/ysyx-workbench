#include <sim_main.h>

VerilatedContext* contextp = nullptr;
VerilatedFstC* tfp = nullptr;
VTop* top;

extern "C" void set_pc_ptr(uint32_t ptr);
extern "C" void set_gpr_ptr();
//extern "C" void set_instr_mem_ptr(uint64_t ptr);

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
  set_pc_ptr((uint64_t)&top->io_pc);
  set_gpr_ptr();
  //set_instr_mem_ptr((uint64_t)&top->io_instr_mem);
}

void sim_exit() {
  tfp->close();
}

void eval_once() {
  //top->io_dpi_mem_req = 1;
  top->io_dpi_mem_addr = mem_read_addr;
  top->eval();  // trigger logic
  mem_read_data = top->io_dpi_mem_data;
  gpr_x0 = top->io_rf_out_0;
  gpr_x1 = top->io_rf_out_1;
  gpr_x2 = top->io_rf_out_2;
  gpr_x3 = top->io_rf_out_3;
  gpr_x4 = top->io_rf_out_4;
  gpr_x5 = top->io_rf_out_5;
  gpr_x6 = top->io_rf_out_6;
  gpr_x7 = top->io_rf_out_7;
  gpr_x8 = top->io_rf_out_8;
  gpr_x9 = top->io_rf_out_9;
  gpr_x10 = top->io_rf_out_10;
  gpr_x11 = top->io_rf_out_11;
  gpr_x12 = top->io_rf_out_12;
  gpr_x13 = top->io_rf_out_13;
  gpr_x14 = top->io_rf_out_14;
  gpr_x15 = top->io_rf_out_15;
  gpr_x16 = top->io_rf_out_16;
  gpr_x17 = top->io_rf_out_17;
  gpr_x18 = top->io_rf_out_18;
  gpr_x19 = top->io_rf_out_19;
  gpr_x20 = top->io_rf_out_20;
  gpr_x21 = top->io_rf_out_21;
  gpr_x22 = top->io_rf_out_22;
  gpr_x23 = top->io_rf_out_23;
  gpr_x24 = top->io_rf_out_24;
  gpr_x25 = top->io_rf_out_25;
  gpr_x26 = top->io_rf_out_26;
  gpr_x27 = top->io_rf_out_27;
  gpr_x28 = top->io_rf_out_28;
  gpr_x29 = top->io_rf_out_29;
  gpr_x30 = top->io_rf_out_30;
  gpr_x31 = top->io_rf_out_31;
  //top->io_dpi_mem_req = 0;
}
#define __GUEST_ISA__ riscv32
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
