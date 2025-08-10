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
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  paddr_write(addr, len, data);
}
/*
word_t mem[MEM_SIZE]; // Memory array to hold the contents of the memory
void init_mem_from_file(const char *filename) {
  //Log("Initializing memory from file: %s\n", filename);
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("open rom.txt failed");
    exit(1);
  }

  vaddr_t addr = 0;
  char line[256];

  while (fgets(line, sizeof(line), fp)) {
    word_t data;
    if (sscanf(line, "%x", &data) != 1) continue;

    // Little-endian write
    mem[addr] = data;

    addr++;
  }
  //print_mem(0, addr - 1);
  fclose(fp);
}

void print_mem(vaddr_t start, vaddr_t end) {
  for (vaddr_t addr = start; addr <= end; addr++) {
    printf("Address 0x%08x: 0x%08x\n", addr, mem[addr]);
  }
}

int pmem_read(int raddr) {
  // Always read 4 bytes aligned to the address `raddr & ~0x3u` and return to `rdata`
  //Log("Reading memory at address: 0x%x", raddr);
  //vaddr_t addr = (raddr - 0x80000000)>>2 & ~0x3u; // align to 4 bytes
  vaddr_t addr = (raddr - 0x80000000)>>2;
  //Log("raddr: 0x%x", raddr);
  //Log("(raddr - 0x80000000)>>2: 0x%x", (raddr - 0x80000000)>>2);
  //Log("addr: 0x%x", addr);
  return mem[addr]; // Return the read data
}
void pmem_write(int waddr, int wdata, char wmask) {
  // Always write `wdata` according to the write mask `wmask` into 4 bytes aligned to the address `waddr & ~0x3u`
  // Each bit in `wmask` represents a mask for 1 byte in `wdata`
  // For example, `wmask = 0x3` means only write the lowest 2 bytes, leaving the other bytes in memory unchanged
  vaddr_t addr = waddr & ~0x3u; // align to 4 bytes
  for (int i = 0; i < 4; i++) {
    if (wmask & (1 << i)) {
      mem[addr] = (wdata >> (i * 8)) & 0xFF;
    }
  }
}
*/
