#include <elf.h>
#include <common.h>
#include <cpu/decode.h>
#define MAX_FUNC_NUM 1024 

typedef struct {
  char *name;
  vaddr_t addr;
  size_t size;
} FuncSym;

extern FuncSym func_table[MAX_FUNC_NUM];
extern int func_cnt;

void ftrace_init(const char *elf_path);
void ftrace_log(vaddr_t pc, vaddr_t target, bool is_call, bool is_ret);
void ftrace_try_log(Decode *s);