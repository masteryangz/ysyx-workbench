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

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.mepc = epc;
  cpu.mcause = NO;
  cpu.mstatus = 0x1800;
#ifdef CONFIG_ETRACE
  //nemu_state.state = NEMU_STOP;
  Etrace("pc = %08x: Exception/Interrupt! mepc = 0x%08x, mstatus = 0x%08x, mcause = %d, mtvec = 0x%08x\n",
    cpu.pc, cpu.mepc, cpu.mstatus, cpu.mcause, cpu.mtvec);
#endif
  return cpu.mtvec;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
