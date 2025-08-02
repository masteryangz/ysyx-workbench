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
#include <memory/vaddr.h>
//#include <memory/paddr.h>

// 全局变量或静态状态
vaddr_t mem_read_addr = 0;
word_t mem_read_data = 0;

// C → Verilog 的接口函数（被 Verilator 调用）
static void set_mem_read_addr(vaddr_t addr) {
  mem_read_addr = addr;
}

static word_t get_mem_read_data() {
  return mem_read_data;
}

// 实现 vaddr_read
word_t vaddr_read(vaddr_t addr) {
  //assert(len == 4);  // 暂时只支持 word 读取
  set_mem_read_addr(addr);

  // 触发 top->eval()，数据才会更新
  extern void eval_once();  // 你需要在 sim_main.cpp 暴露这个
  eval_once();

  return get_mem_read_data();
}
/*
word_t vaddr_ifetch(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}
*/
/*
void vaddr_write(vaddr_t addr, int len, word_t data) {
  paddr_write(addr, len, data);
}
*/