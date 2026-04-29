#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;
#include <stdio.h>
#include <stddef.h>

static void dump_memory_word(void *addr, int words) {
  uint32_t *p = (uint32_t*)addr;
  for (int i = 0; i < words; i++) {
    printf("%x ", p[i]);
    if ((i+1) % 8 == 0) printf("\n");
  }
  if (words % 8) printf("\n");
}

void etrace_ctx(Context *c) {
  printf("==== ETRACE CONTEXT @ %p ====\n", (void*)c);
  // print a few header fields commonly used
  printf("mepc = 0x%x, mcause = 0x%x, mstatus = 0x%x\n",
         (unsigned)c->mepc, (unsigned)c->mcause, (unsigned)c->mstatus);
  printf("---- stack top words (from context address) ----\n");
  dump_memory_word((void*)c, 32); // dump first 32 words at context memory for raw view
  printf("==============================================\n");
}

Context* __am_irq_handle(Context *c) {
  //etrace_ctx(c);

  if (user_handler) {
    Event ev = {0};
    //switch (c->mcause) {
#ifdef __riscv_e
    if (c->gpr[15] == -1) {
#else
    if (c->gpr[17] == -1) {
#endif  
      //case EVENT_IRQ_TIMER: ev.event = EVENT_IRQ_TIMER; break;
      //case EVENT_IRQ_IODEV: ev.event = EVENT_IRQ_IODEV; break;
      ev.event = EVENT_YIELD;
      c->mepc += 4;
      /*
      case 11: 
        ev.event = EVENT_YIELD;
        c->mepc += 4;
        break;
      */
      //case EVENT_SYSCALL: ev.event = EVENT_SYSCALL; break;
      //case EVENT_PAGEFAULT: ev.event = EVENT_PAGEFAULT; break;
      //default: ev.event = EVENT_ERROR; break;
    } else {
      ev.event = EVENT_ERROR;
    }
    /*
    printf("mcause = 0x%x, mstatus = 0x%x, mepc = 0x%x\n",
       c->mcause, c->mstatus, c->mepc);
    for(int i = 0; i < 32; i++) {
        printf("x%d = 0x%x\n", i, c->gpr[i]);
    }
    */
    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  // 把 Context 放在栈顶
  Context *c = (Context *)(kstack.end - sizeof(Context));

  // 清空一下，避免脏数据
  memset(c, 0, sizeof(Context));

  // 设置初始寄存器状态
  c->mepc = (uintptr_t)entry;   // 入口地址
  c->gpr[1] = 0;
  c->gpr[2]  = (uintptr_t)c;
  c->gpr[10] = (uintptr_t)arg;  // a0 = arg
  c->mstatus = 0x1800;

  return c;
  //return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}