#include "verilated.h"
#include "verilated_fst_c.h"
#include "../obj_dir/Valu.h"

VerilatedContext* contextp = NULL;
VerilatedFstC* tfp = NULL;

static Valu* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedFstC;
  top = new Valu;
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

   // ===== 测试用例 1：A=0, B=0, Func=000（加法） =====
    top->A = 0b00000; // A = 0
    top->B = 0b00000; // B = 0
    top->Func = 0b000; // 加法
    step_and_dump_wave();

    // ===== 测试用例 2：A=1, B=1, Func=000（加法） =====
    top->A = 0b00001; // A = 1
    top->B = 0b00001; // B = 1
    top->Func = 0b000; // 加法
    step_and_dump_wave();

    // ===== 测试用例 3：A=15, B=15, Func=000（加法） =====
    top->A = 0b01111; // A = 15
    top->B = 0b01111; // B = 15
    top->Func = 0b000; // 加法
    step_and_dump_wave();

    // ===== 测试用例 4：A=15, B=1, Func=000（加法） =====
    top->A = 0b01111; // A = 15
    top->B = 0b00001; // B = 1
    top->Func = 0b000; // 加法
    step_and_dump_wave();

    // ===== 测试用例 5：A=1, B=1, Func=001（减法） =====
    top->A = 0b00001; // A = 1
    top->B = 0b00001; // B = 1
    top->Func = 0b001; // 减法
    step_and_dump_wave();

    // ===== 测试用例 6：A=1, B=-1, Func=001（减法） =====
    top->A = 0b00001; // A = 1
    top->B = 0b11111; // B = -1
    top->Func = 0b001; // 减法
    step_and_dump_wave();

    // ===== 测试用例 7：A=-2, B=15, Func=001（减法） =====
    top->A = 0b11110; // A = -2
    top->B = 0b01111; // B = 15
    top->Func = 0b001; // 减法
    step_and_dump_wave();

    // ===== 测试用例 8：A=-1, B=16, Func=001（减法） =====
    top->A = 0b00001; // A = 1
    top->B = 0b10000; // B = -16
    top->Func = 0b001; // 减法
    step_and_dump_wave();

    // ===== 测试用例 9：A=1, B=0, Func=010（取反） =====
    top->A = 0b00001; // A = 1
    top->B = 0b00000; // B = 0（取反操作忽略 B）
    top->Func = 0b010; // 取反
    step_and_dump_wave();

    // ===== 测试用例 10：A=1, B=0, Func=011（与） =====
    top->A = 0b00001; // A = 1
    top->B = 0b00001; // B = 1
    top->Func = 0b011; // 与
    step_and_dump_wave();

    // ===== 测试用例 11：A=1, B=0, Func=100（或） =====
    top->A = 0b00001; // A = 1
    top->B = 0b00001; // B = 1
    top->Func = 0b100; // 或
    step_and_dump_wave();

    // ===== 测试用例 12：A=1, B=0, Func=101（异或） =====
    top->A = 0b00001; // A = 1
    top->B = 0b00001; // B = 1
    top->Func = 0b101; // 异或
    step_and_dump_wave();

    // ===== 测试用例 13：A=1, B=2, Func=110（比较 A < B） =====
    top->A = 0b00001; // A = 1
    top->B = 0b00010; // B = 2
    top->Func = 0b110; // 比较 A < B
    step_and_dump_wave();

    // ===== 测试用例 14：A=1, B=1, Func=111（判断 A == B） =====
    top->A = 0b00001; // A = 1
    top->B = 0b00001; // B = 1
    top->Func = 0b111; // 判断 A == B
    step_and_dump_wave();
  sim_exit();
}