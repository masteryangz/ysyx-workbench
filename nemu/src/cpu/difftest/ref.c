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

typedef struct {
  word_t gpr[32];
  word_t pc;
} riscv32_difftest_regs_t;

__EXPORT void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
  //assert(0);
  if (direction == DIFFTEST_TO_REF) {
    word_t *p = (word_t *)buf;
    for (size_t i = 0; i < n; i++) {
      paddr_write((paddr_t)(addr + i), 4, p[i]);
    }
  } else {
    word_t *p = (word_t *)buf;
    for (size_t i = 0; i < n; i++) {
      p[i] = (word_t)paddr_read((paddr_t)(addr + i), 4);
    }
  }
}

__EXPORT void difftest_regcpy(void *dut, bool direction) {
  //assert(0);
  riscv32_difftest_regs_t *r = (riscv32_difftest_regs_t *)dut;
  if (direction == DIFFTEST_TO_REF) {
    for (int i = 0; i < 32; i++) cpu.gpr[i] = r->gpr[i];
    cpu.pc = r->pc;
  } else {
    for (int i = 0; i < 32; i++) r->gpr[i] = cpu.gpr[i];
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
