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

#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
word_t expr(char *, bool *);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif
/*
  FILE *file = fopen("tools/gen-expr/build/input", "r");
  if (file == NULL) {
      perror("Error opening file");
      return 1;
  }
  // Buffer to hold each line (adjust size as needed)
  int size = 65536;
  char line[size];
  char fresult[size];
  char fexpression[size];
  // Read file line by line
  while (fgets(line, sizeof(line), file)) {
    // Find the first occurrence of ' '
    char *space_ptr = strchr(line, ' ');
    // Split the line into result and expression

    // Copy the chunk before the space into 'result'
    strncpy(fresult, line, space_ptr - line);
    fresult[space_ptr - line] = '\0';  // Ensure null termination

    // Copy the rest of the line (after the space) into 'expression'
    strcpy(fexpression, space_ptr + 1);  // Skip the space itself
    //Log("fresult = %s, fexpression = %s", fresult, fexpression);
    bool expr_success;
    word_t aresult = expr(fexpression, &expr_success);
    Log("expr_success = %d, fexpression = %s", expr_success, fexpression);
    if (!expr_success) {
      Log("Expression evaluation failed for: %s", fexpression);
      continue;  // Skip to the next line if evaluation fails
    }
    // Convert fresult to a number
    word_t expected_result = strtol(fresult, NULL, 10);
    // Compare the expected result with the evaluated result
    Log("Expression: %s, Expected: %" PRIu32", Evaluated: %" PRIu32"\n", fexpression, expected_result, aresult);
    assert(expected_result == aresult);
  }

  // Check for errors during reading
  if (ferror(file)) {
      perror("Error reading file");
  }

  // Close the file
  fclose(file);
  */
  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
