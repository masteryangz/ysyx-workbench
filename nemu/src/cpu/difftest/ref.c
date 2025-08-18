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
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>

__EXPORT void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
  //assert(0);
  //Log("direction = %s", (direction == DIFFTEST_TO_REF) ? "DIFFTEST_TO_REF" : "DIFFTEST_TO_DUT");
  if (direction == DIFFTEST_TO_REF) {
    word_t *p = (word_t *)buf;
    Log("p = %08x", p[1]);
    for (size_t i = 0; i < n; i++) {
      paddr_write((paddr_t)(addr + 4*i), 4, p[i]);
    }
  } else {
    word_t *p = (word_t *)buf;
    for (size_t i = 0; i < n; i++) {
      p[i] = (word_t)paddr_read((paddr_t)(addr + 4*i), 4);
    }
  }
  //print_mem(0, 12);
}

__EXPORT void difftest_regcpy(void *dut, bool direction) {
  //assert(0);
  //Log("direction = %s", (direction == DIFFTEST_TO_REF) ? "DIFFTEST_TO_REF" : "DIFFTEST_TO_DUT");
  CPU_state *r = (CPU_state *)dut;
  if (direction == DIFFTEST_TO_REF) {
    for (int i = 0; i < 32; i++) cpu.gpr[i] = r->gpr[i];
    cpu.pc = r->pc;
  } else {
    for (int i = 0; i < 32; i++) {
      r->gpr[i] = cpu.gpr[i];
      //Log("cpu.gpr[%d] = %08x", i, cpu.gpr[i]);
    }
    r->pc = cpu.pc;
  }
}

__EXPORT void difftest_exec(uint64_t n) {
  //assert(0);
  cpu_exec(n);
}

__EXPORT void difftest_raise_intr(word_t NO) {
  assert(0);
}

__EXPORT void difftest_init(int port) {
  void init_mem();
  init_mem();
  /* Perform ISA dependent initialization. */
  init_isa();
}
