#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/Vpr_encoder.h"

VerilatedContext* contextp = NULL;
VerilatedFstC* tfp = NULL;

static Vpr_encoder* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedFstC;
  top = new Vpr_encoder;
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

  top->en=0b0; top->x =0b00000000; step_and_dump_wave();
               top->x =0b00000001; step_and_dump_wave();
               top->x =0b00000010; step_and_dump_wave();
               top->x =0b00000101; step_and_dump_wave();
               top->x =0b00001010; step_and_dump_wave();
               top->x =0b00010101; step_and_dump_wave();
               top->x =0b00101010; step_and_dump_wave();
               top->x =0b01010101; step_and_dump_wave();
               top->x =0b10101010; step_and_dump_wave();
  top->en=0b1; top->x =0b00000000; step_and_dump_wave();
               top->x =0b00000001; step_and_dump_wave();
               top->x =0b00000010; step_and_dump_wave();
               top->x =0b00000101; step_and_dump_wave();
               top->x =0b00001010; step_and_dump_wave();
               top->x =0b00010101; step_and_dump_wave();
               top->x =0b00101010; step_and_dump_wave();
               top->x =0b01010101; step_and_dump_wave();
               top->x =0b10101010; step_and_dump_wave();
  sim_exit();
}