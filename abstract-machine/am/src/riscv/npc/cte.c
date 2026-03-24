#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 11: 
        ev.event = EVENT_YIELD;
        c->mepc += 4;
        //printf("yield event, mepc = %08x\n", c->mepc);
        break;
      default: ev.event = EVENT_ERROR; break;
    }

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
