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

#include <reg/reg.h>
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>

static void decode_operand(Decode *s, int *rd, word_t *src1, word_t *src2, word_t *imm, int type) {
}

static int decode_exec(Decode *s) {
  //s->dnpc = s->snpc;
  //Log("s->dnpc = %08x, s->snpc = %08x", s->dnpc, s->snpc);
  //Log("Sim time: %lu", sim_time);
  top->clock = 0; step_and_dump_wave();
  //Log("half step");
  top->clock = 1; step_and_dump_wave();
  //Log("one step");
  //cpu.pc = get_pc();
  s->dnpc = get_pc();
  //Log("cpu.pc = %08x", cpu.pc);
  for (int i = 0; i < 32; i++) {
    cpu.gpr[i] = get_gpr(i);
  }
  return 0;
}

int isa_exec_once(Decode *s) {
  //Log("s->snpc = %08x", s->snpc);
  s->isa.inst = inst_fetch(&s->snpc, 4);
  //Log("%08x\n", s->isa.inst);
  //ringbuf_push(&iringbuf, s->snpc, s->isa.inst);
  //uint8_t *code = (uint8_t *)&s->isa.inst;
  //ringbuf_push(&iringbuf, s->snpc, code, 4);
  return decode_exec(s);
}
