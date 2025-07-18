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
#include "local-include/reg.h"

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
    printf("%s: 0x%08x\n", regs[i], value);
  }
}

word_t isa_reg_str2val(const char *s, bool *success) {
  //Log("size of regs: %zu", sizeof(regs) / sizeof(regs[0]));
  if (strcmp(s, "pc") == 0) {
    *success = true;
    return cpu.pc; // Return the program counter value
  }
  for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    if (strcmp(s, regs[i]) == 0) {
      *success = true;
      return cpu.gpr[i];
    }
  }
  *success = false;
  return 0; // Return 0 if the register name is not found
}
