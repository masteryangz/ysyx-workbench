#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/Vlfsr.h"

VerilatedContext* contextp = NULL;
VerilatedFstC* tfp = NULL;

static Vlfsr* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedFstC;
  top = new Vlfsr;
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
 // ===== 测试用例 1：初始状态，触发一次 LFSR =====
  printf("=== Test Case 1: Trigger LFSR once ===\n");

  top->btn_clk = 1;  // 触发
  top->clk = 1;
  step_and_dump_wave();
  top->btn_clk = 0;
  top->clk = 0;
  step_and_dump_wave();

  printf("HEX0 = %02x\n", top->hex0);
  printf("HEX1 = %02x\n", top->hex1);

  // ===== 测试用例 2：再触发一次 =====
  printf("\n=== Test Case 2: Trigger LFSR again ===\n");

  top->btn_clk = 1;
  top->clk = 1;
  step_and_dump_wave();
  top->btn_clk = 0;
  top->clk = 0;
  step_and_dump_wave();

  printf("HEX0 = %02x\n", top->hex0);
  printf("HEX1 = %02x\n", top->hex1);

  // ===== 测试用例 3：再触发一次 =====
  printf("\n=== Test Case 3: Trigger LFSR third time ===\n");

  top->btn_clk = 1;
  top->clk = 1;
  step_and_dump_wave();
  top->btn_clk = 0;
  top->clk = 0;
  step_and_dump_wave();

  printf("HEX0 = %02x\n", top->hex0);
  printf("HEX1 = %02x\n", top->hex1);
  // ===== 测试用例 4：再触发一次 =====
  printf("\n=== Test Case 4: Trigger LFSR ===\n");

  top->btn_clk = 1;
  top->clk = 1;
  step_and_dump_wave();
  top->btn_clk = 0;
  top->clk = 0;
  step_and_dump_wave();

  printf("HEX0 = %02x\n", top->hex0);
  printf("HEX1 = %02x\n", top->hex1);
  // ===== 测试用例 5：再触发一次 =====
  printf("\n=== Test Case 5: Trigger LFSR ===\n");

  top->btn_clk = 1;
  top->clk = 1;
  step_and_dump_wave();
  top->btn_clk = 0;
  top->clk = 0;
  step_and_dump_wave();

  printf("HEX0 = %02x\n", top->hex0);
  printf("HEX1 = %02x\n", top->hex1);
    // ===== 测试用例 6：再触发一次 =====
  printf("\n=== Test Case 6: Trigger LFSR ===\n");

  top->btn_clk = 1;
  top->clk = 1;
  step_and_dump_wave();
  top->btn_clk = 0;
  top->clk = 0;
  step_and_dump_wave();

  printf("HEX0 = %02x\n", top->hex0);
  printf("HEX1 = %02x\n", top->hex1);
    // ===== 测试用例 7：再触发一次 =====
  printf("\n=== Test Case 7: Trigger LFSR ===\n");

  top->btn_clk = 1;
  top->clk = 1;
  step_and_dump_wave();
  top->btn_clk = 0;
  top->clk = 0;
  step_and_dump_wave();

  printf("HEX0 = %02x\n", top->hex0);
  printf("HEX1 = %02x\n", top->hex1);
    // ===== 测试用例 8：再触发一次 =====
  printf("\n=== Test Case 8: Trigger LFSR ===\n");

  top->btn_clk = 1;
  top->clk = 1;
  step_and_dump_wave();
  top->btn_clk = 0;
  top->clk = 0;
  step_and_dump_wave();

  printf("HEX0 = %02x\n", top->hex0);
  printf("HEX1 = %02x\n", top->hex1);
  sim_exit();
}