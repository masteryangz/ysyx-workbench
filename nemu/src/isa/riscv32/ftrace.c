#include <ftrace.h>

int func_cnt = 0;
FuncSym func_table[MAX_FUNC_NUM];

void ftrace_init(const char *elf_path) {
    //Log("elf_path = %s", elf_path);
    FILE *fp = fopen(elf_path, "rb");
    assert(fp);

    // read ELF header
    Elf32_Ehdr ehdr;
    if (fread(&ehdr, sizeof(ehdr), 1, fp) != 1) { 
      perror("fread failed"); 
      fclose(fp); 
      return; 
    }
    //fread(&ehdr, sizeof(ehdr), 1, fp);

    // seek section headers
    fseek(fp, ehdr.e_shoff, SEEK_SET);
    Elf32_Shdr sh_table[ehdr.e_shnum];
    if (fread(sh_table, sizeof(Elf32_Shdr), ehdr.e_shnum, fp) != ehdr.e_shnum) { 
      perror("fread failed"); 
      fclose(fp); 
      return; 
    }
    //fread(sh_table, sizeof(Elf32_Shdr), ehdr.e_shnum, fp);

    // Read section header string table
    if (ehdr.e_shstrndx >= ehdr.e_shnum) {
        printf("FTRACE ERROR: Invalid section header string table index\n");
        fclose(fp);
        return;
    }
    Elf32_Shdr sh_str = {0};
    sh_str = sh_table[ehdr.e_shstrndx];
    char *shstrtab = malloc(sh_str.sh_size);
    fseek(fp, sh_str.sh_offset, SEEK_SET);
    if (fread(shstrtab, sh_str.sh_size, 1, fp) != 1) { 
      perror("fread failed"); 
      free(shstrtab);
      fclose(fp); 
      return; 
    }
    //fread(shstrtab, sh_str.sh_size, 1, fp);

    // Add flags here 
    bool found_symtab = false;
    bool found_strtab = false;

    // Find symbol and string table
    Elf32_Shdr symtab = {0}, strtab = {0};  // zero-init for safety
    for (int i = 0; i < ehdr.e_shnum; ++i) {
        if (sh_table[i].sh_type == SHT_SYMTAB) {
            symtab = sh_table[i];
            if (symtab.sh_link >= ehdr.e_shnum) {
                printf("FTRACE ERROR: symtab.sh_link out of bounds.\n");
                fclose(fp);
                return;
            }
            found_symtab = true;

            strtab = sh_table[symtab.sh_link];  // linked string table
            found_strtab = true;
            break;
        }
    }

    // Abort if missing required tables
    if (!found_symtab || !found_strtab) {
        printf("FTRACE ERROR: symbol or string table not found!\n");
        fclose(fp);
        free(shstrtab);
        return;
    }

    // Read symbol table
    int sym_num = symtab.sh_size / symtab.sh_entsize;
    //Log("ftrace: sym_num = %d", sym_num);
    Elf32_Sym *symtab_data = malloc(symtab.sh_size);
    fseek(fp, symtab.sh_offset, SEEK_SET);
    if (fread(symtab_data, symtab.sh_size, 1, fp) != 1) { 
      perror("fread failed"); 
      free(shstrtab);
      free(symtab_data);
      fclose(fp); 
      return; 
    }
    //fread(symtab_data, symtab.sh_size, 1, fp);

    // Read string table
    char *strtab_data = malloc(strtab.sh_size);
    fseek(fp, strtab.sh_offset, SEEK_SET);
    if (fread(strtab_data, strtab.sh_size, 1, fp) != 1) { 
      perror("fread failed"); 
      free(strtab_data);
      free(shstrtab);
      free(symtab_data);
      fclose(fp); 
      return; 
    }
    //fread(strtab_data, strtab.sh_size, 1, fp);

    // Extract function symbols
    for (int i = 0; i < sym_num; i++) {
        //Log("ftrace: ELF32_ST_TYPE(symtab_data[i].st_info) %s STT_FUNC", ELF32_ST_TYPE(symtab_data[i].st_info) == STT_FUNC ? "==" : "!=");
        if (ELF32_ST_TYPE(symtab_data[i].st_info) == STT_FUNC &&
            symtab_data[i].st_size > 0) {
            func_table[func_cnt].addr = symtab_data[i].st_value;
            func_table[func_cnt].size = symtab_data[i].st_size;
            func_table[func_cnt].name = strdup(strtab_data + symtab_data[i].st_name);
            //Log("ftrace: %s at 0x%08x, size %zu",
            //    func_table[func_cnt].name, func_table[func_cnt].addr, func_table[func_cnt].size);
            func_cnt++;
        }
    }

    free(shstrtab);
    free(symtab_data);
    free(strtab_data);
    fclose(fp);
}


int call_depth = 0;

void ftrace_log(vaddr_t pc, vaddr_t target, bool is_call, bool is_ret) {
  const char *callee = NULL;
  for (int i = 0; i < func_cnt; i++) {
    if (target >= func_table[i].addr && 
        target < func_table[i].addr + func_table[i].size) {
      callee = func_table[i].name;
      break;
    }
  }

  if (callee) {
    //Log("callee is %s", callee);
    if (is_call) {
      printf("[ftrace] %*sCall %s@0x%x\n", call_depth * 2, "", callee, target);
      call_depth++;
    } else if (is_ret) {
      call_depth--;
      printf("[ftrace] %*sReturn from %s\n", call_depth * 2, "", callee);
    }
  }
}

void ftrace_try_log(Decode *s) {
  uint32_t inst = s->isa.inst;
  vaddr_t pc = s->pc;
  vaddr_t target = s->dnpc;

  uint32_t opcode = inst & 0x7f;
  uint32_t rd = (inst >> 7) & 0x1f;
  uint32_t rs1 = (inst >> 15) & 0x1f;
  uint32_t funct3 = (inst >> 12) & 0x7;
  if (opcode == 0x6f && rd == 1) {
    // jal x1, imm —> direct function call
    ftrace_log(pc, target, true, false);
  }
  else if (opcode == 0x67 && rs1 == 1 && rd == 0 && funct3 == 0) {
    // jalr x0, x1, 0 —> return
    ftrace_log(pc, target, false, true);
  }
  else if (opcode == 0x67 && rd == 1) {
    // jalr x1, xN, imm —> indirect function call
    ftrace_log(pc, target, true, false);
  }
}
