/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <reg/reg.h>
//#include <sim_main.h>
#define GET_RF_CASE(i) case i: return top->rootp->Top__DOT__id__DOT__regfile__DOT__rf_##i;
const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    bool success = false;
    word_t value = isa_reg_str2val(regs[i], &success);
    if (!success) {
      printf("Error: Register %s not found\n", regs[i]);
      continue; // Skip to the next register if the current one is not found
    }
    printf("%s: 0x%08x ", regs[i], value);
    if (i % 4 == 3) {
      printf("\n"); // Print a newline after every 4 registers
    }
  }
}

word_t isa_reg_str2val(const char *s, bool *success) {
  /*
  svScope scope = svGetScope();
  if (!scope) {
    printf("%%Warning: svGetScope failed\n");
    exit(1);
  }*/
  if (strcmp(s, "pc") == 0) {
    *success = true;
    return get_pc();
    //return cpu.pc; // Return the program counter value
  }
  for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    if (strcmp(s, regs[i]) == 0) {
      *success = true;
      //Log("Getting reg %s", s);
      return get_gpr(i);
      //return cpu.gpr[i];
    }
  }
  *success = false;
  return 0; // Return 0 if the register name is not found
}

word_t get_pc() {
  //Log("Getting pc = 0x%08x", top->rootp->Top__DOT__instrfet__DOT__pcReg);
  return top->rootp->Top__DOT__instrfet__DOT__pcReg;
}

word_t get_gpr(int idx) {
  if (idx < 0 || idx >= 32) {
    printf("Error: Invalid register index %d\n", idx);
    return 0; // Return 0 for invalid index
  }
  switch(idx) {
    GET_RF_CASE(0)
    GET_RF_CASE(1)
    GET_RF_CASE(2)
    GET_RF_CASE(3)
    GET_RF_CASE(4)
    GET_RF_CASE(5)
    GET_RF_CASE(6)
    GET_RF_CASE(7)
    GET_RF_CASE(8)
    GET_RF_CASE(9)
    GET_RF_CASE(10)
    GET_RF_CASE(11)
    GET_RF_CASE(12)
    GET_RF_CASE(13)
    GET_RF_CASE(14)
    GET_RF_CASE(15)
    GET_RF_CASE(16)
    GET_RF_CASE(17)
    GET_RF_CASE(18)
    GET_RF_CASE(19)
    GET_RF_CASE(20)
    GET_RF_CASE(21)
    GET_RF_CASE(22)
    GET_RF_CASE(23)
    GET_RF_CASE(24)
    GET_RF_CASE(25)
    GET_RF_CASE(26)
    GET_RF_CASE(27)
    GET_RF_CASE(28)
    GET_RF_CASE(29)
    GET_RF_CASE(30)
    GET_RF_CASE(31)
    default:
      printf("Error: Invalid register index %d\n", idx);
      return 0; // Return 0 for invalid index
  }
}