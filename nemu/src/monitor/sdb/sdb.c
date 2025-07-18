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
#include <memory/vaddr.h>
#include <monitor/sdb/watchpoint.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);

static int cmd_info(char *args) {
  if (args == NULL) {
    printf("Usage: info [r|w]<reg|watchpoint>\n");
    return 0;
  }

  if (strcmp(args, "r") == 0) {
    isa_reg_display();
  }
  else if (strcmp(args, "w") == 0) {
    for (WP *wp = wp_head; wp != NULL; wp = wp->next) {
      printf("Watchpoint %d: %s = 0x%08x\n", wp->NO, wp->expr, wp->value);
    }
  }
  else {
    printf("Unknown info command: %s\n", args);
  }

  return 0;
}

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Execute one instruction and then stop", cmd_si },
  { "info", "Show information about registers, watchpoints, etc. Usage: info [r|w|]", cmd_info },
  { "p", "Evaluate an expression and print the result. Usage: p <expression>", cmd_p },
  { "x", "Display memory content. Usage: x <iterations> <expression>", cmd_x },
  { "w", "Set a watchpoint. Usage: w <expression>", cmd_w },
  { "d", "Delete a watchpoint. Usage: d <watchpoint number>", cmd_d },
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_p(char *args) {
  if (args == NULL) {
    printf("Usage: p <expression>\n");
    return 0;
  }

  bool success = false;
  word_t result = expr(args, &success);
  if (!success) {
    printf("Expression evaluation failed: %s\n", args);
    return 0;
  }

  printf("Result: 0x%08x\n", result);
  return 0;
}

static int cmd_w(char *args) {
  if (args == NULL) {
    printf("Usage: w <expression>\n");
    return 0;
  }

  bool success = false;
  word_t value = expr(args, &success);
  if (!success) {
    printf("Expression evaluation failed: %s\n", args);
    return 0;
  }

  WP *wp = new_wp(args, value);
  if (wp == NULL) {
    printf("Failed to create watchpoint for expression: %s\n", args);
    return 0;
  }
  assert(wp_head != NULL);
  printf("Watchpoint %d created: %s = 0x%08x\n", wp->NO, wp->expr, wp->value);
  for (WP *wp_iter = wp_head; wp_iter != NULL; wp_iter = wp_iter->next) {
    //Log("Watchpoint %d: %s = 0x%08x\n", wp_iter->NO, wp_iter->expr, wp_iter->value);
  }
  return 0;
}

static int cmd_d(char *args) {
  if (args == NULL) {
    printf("Usage: d <watchpoint number>\n");
    return 0;
  }

  int wp_no = 0;
  if (sscanf(args, "%d", &wp_no) != 1 || wp_no < 0) {
    printf("Invalid watchpoint number: %s\n", args);
    return 0;
  }
  WP *wp = NULL;
  for (wp = wp_head; wp != NULL; wp = wp->next) {
    if (wp->NO == wp_no) {
      break;
    }
  }
  if (wp == NULL) {
    printf("No watchpoint found with number %d\n", wp_no);
    return 0;
  }

  free_wp(wp);
  printf("Watchpoint %d deleted.\n", wp_no);
  return 0;
}

static int cmd_si(char *args) {
  int n = 1; // default to execute one instruction
  char *token;
  if (args != NULL) {
    token = strtok(args, " ");
    if (token == NULL || sscanf(token, "%d", &n) <= 0 || n <= 0) {
      printf("Invalid argument for si: %s\n", args);
      return 0;
    }
  }
  cpu_exec(n);
  return 0;
}

static int cmd_x(char *args) {
  if (args == NULL) {
    printf("Usage: x <iterations> <expression>\n");
    return 0;
  }

  char *n_token = strtok(args, " ");
  char *ex_token = strtok(NULL, " ");
  int n = 0;

  if (n_token == NULL || ex_token == NULL) {
    printf("Usage: x <iterations> <expression>\n");
    return 0;
  }

  if (sscanf(n_token, "%d", &n) != 1 || n <= 0) {
    printf("Invalid number of iterations: %s\n", n_token);
    return 0;
  }

  for (int i = 0; i < n; i++) {
    bool success = false;
    word_t result = expr(ex_token, &success);
    if (!success) {
      printf("Expression evaluation failed at address 0x%s\n", ex_token + i * 4);
      return 0;
    }
    word_t data = vaddr_read(result + i * 4, 4);
    printf("%s + %d word_t: %08x\n", ex_token, i, data);
  }

  return 0;
}

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
