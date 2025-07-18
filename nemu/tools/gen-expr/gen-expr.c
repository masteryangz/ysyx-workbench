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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static int ind = 0;
static bool is_div = false;
static char *code_format =
"#include <stdio.h>\n"
"#include <stdint.h>\n"
"int main() { "
"  uint32_t result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static void gen_rand_op() {
  if (ind >= (int)sizeof(buf) - 1) {
    return; // Buffer full
  }
  switch (rand() % 4) {
    case 0: buf[ind++] = '+'; is_div = false; break;
    case 1: buf[ind++] = '-'; is_div = false; break;
    case 2: buf[ind++] = '*'; is_div = false; break;
    case 3: buf[ind++] = '/'; is_div = true; break;
    default: assert(0); // should not happen
  }
}

static void gen_num() {
    uint32_t num = 0;
    while (num == 0) { // Ensure non-zero
        num = rand() & 0xFFFFFFFF;
    }
    
    size_t remaining_space = sizeof(buf) - ind;
    if (remaining_space == 0) {
        return; // Buffer full
    }

    int len = snprintf(buf + ind, remaining_space, "%uu", num);

    if (len < 0 || (size_t)len >= remaining_space) {
        return;
    }

    ind += len;
}

static void gen_space() {
  uint32_t space_count = rand() % 5;
  while (space_count > 0 && ind < (int)sizeof(buf) - 1) {
    buf[ind++] = ' ';
    space_count--;
  }
}

static void gen_rand_expr(bool possible_divisor) {
  if (possible_divisor && is_div) {
    gen_num();
    buf[ind] = '\0';
    return; // Avoid division by zero
  }
  if (ind >= (int)sizeof(buf) - 1) return; // Buffer full
  switch (rand() % 3) {
    case 0:
      gen_num();
      buf[ind] = '\0';
      break;
    case 1:
      if (ind < (int)sizeof(buf) - 2) { // Need space for '(' and ')'
        buf[ind++] = '(';
        gen_rand_expr(false);
        buf[ind++] = ')';
        buf[ind] = '\0';
      }
      break;
    //default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); buf[ind] = '\0'; break;
    default: gen_space(); gen_rand_expr(false); gen_space(); gen_rand_op(); gen_space(); gen_rand_expr(true); gen_space(); buf[ind] = '\0'; break;
  }
}
/*
static void gen_rand_expr() {
  buf[0] = '\0';
}
*/
int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr(false);
    ind = 0; // Reset index for next expression
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
