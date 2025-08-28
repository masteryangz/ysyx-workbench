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

#include <memory/host.h>
#include <memory/paddr.h>
//#include <device/mmio.h>
#include <isa.h>

//#if   defined(CONFIG_PMEM_MALLOC)
//static uint8_t *pmem = NULL;
//#else // CONFIG_PMEM_GARRAY
//static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
//#endif

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

word_t pmem_read(paddr_t addr) {
  //Log("addr = %08x", addr);
  word_t ret = host_read(guest_to_host(addr & ~0x3u), 4);
  //word_t ret = host_read(guest_to_host(addr), 4);
#ifdef CONFIG_MTRACE
    Mtrace("[mtrace] LOAD 0x%08x: addr=0x%08x data=0x%08x width=%d\n",
      cpu.pc, addr, ret, 4);
#endif
  //Log("ret = %" PRIX32 "\n", ret);
  //Log("CONFIG_MBASE = %08x", CONFIG_MBASE);
  return ret;
}

void pmem_write(paddr_t addr, word_t data, char wmask) {
  word_t wdata = 0;
  int len;
  if (wmask == 0b1111) len = 4;
  else if (wmask == 0b11) len = 2;
  else len = 1;
  for (int i = 0; i < 4; i++) {
    if (wmask & (1 << i)) {
      wdata = wdata | (data & (0xFF << (8 * i)));
    }
  }
  //Log("wdata = %08x, wmask = %d", wdata, wmask);
  //host_write(guest_to_host(addr & ~0x3u), len, wdata);
  host_write(guest_to_host(addr), len, wdata);
#ifdef CONFIG_MTRACE
  Mtrace("[mtrace] STORE 0x%08x: addr=0x%08x data=0x%08x width=%d\n",
  cpu.pc, addr, data, len);
#endif
}

static void out_of_bound(paddr_t addr) {
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
      addr, PMEM_LEFT, PMEM_RIGHT, cpu.pc);
}

void init_mem() {
#if   defined(CONFIG_PMEM_MALLOC)
  pmem = malloc(CONFIG_MSIZE);
  assert(pmem);
#endif
  IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
  Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
}

word_t paddr_read(paddr_t addr, int len) {
  if (likely(in_pmem(addr))) {
/*
#ifdef CONFIG_MTRACE
    Mtrace("[mtrace] LOAD 0x%08x: addr=0x%08x data=0x%08x width=%d\n",
      cpu.pc, addr, pmem_read(addr), len);
#endif
*/
    //Log("addr = %08x", addr);
    return pmem_read(addr);
  }
#ifdef CONFIG_DEVICE
#ifdef CONFIG_MTRACE
  Mtrace("[mtrace] LOAD 0x%08x: addr=0x%08x data=0x%08x width=%d\n",
    cpu.pc, addr, pmmio_read(addr, len), len);
#endif
  return mmio_read(addr, len);
#endif
  //IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
  out_of_bound(addr);
  return 0;
}

void paddr_write(paddr_t addr, int len, word_t data) {
  char wmask;
  switch(len) {
    case 1:
      wmask = 0b1;
      break;
    case 2:
      wmask = 0b11;
      break;
    case 4:
      wmask = 0b1111;
      break;
    default:
      wmask = 0b1111;
      break;
  }
/*
#ifdef CONFIG_MTRACE
  Mtrace("[mtrace] STORE 0x%08x: addr=0x%08x data=0x%08x width=%d\n",
  cpu.pc, addr, data, len);
#endif
*/
  if (likely(in_pmem(addr))) { pmem_write(addr, data, wmask); return; }
  IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
  out_of_bound(addr);
}

void print_mem(paddr_t start, paddr_t end) {
  for (paddr_t addr = start; addr < end; addr = addr+4) {
    Log("Address 0x%08x: 0x%02x %02x %02x %02x", addr, (uint8_t)pmem[addr+3], (uint8_t)pmem[addr+2], (uint8_t)pmem[addr+1], (uint8_t)pmem[addr]);
  }
}

/*
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