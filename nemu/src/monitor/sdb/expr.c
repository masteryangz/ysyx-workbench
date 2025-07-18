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
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ = 257, TK_DEC = 258, TK_NEQ = 259, TK_AND = 260, TK_HEX = 261, TK_REG = 262, TK_DEREF = 263
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"u", TK_NOTYPE},     // unused token, for compatibility
  {"\n", TK_NOTYPE},    // newline
  {"\\+", '+'},         // plus
  {"-", '-'},           // minus
  {"\\*", '*'},         // multiply
  {"/", '/'},           // divide
  {"\\(", '('},         // left parenthesis
  {"\\)", ')'},         // right parenthesis
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},       // not equal
  {"&&", TK_AND},       // logical AND
  {"0[xX][0-9a-fA-F]+", TK_HEX}, // hexadecimal number
  {"\\$\\$?[a-zA-Z0-9]+", TK_REG}, // register name
  {"[0-9]+", TK_DEC}    // decimal number
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

static bool make_token(char *e);

static bool check_parentheses(int p, int q, bool *legal);

//word_t expr(char *e, bool *success);

word_t eval(int p, int q, bool *eval_success);
/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
  /* Uncomment the following lines to test the regex compilation.
  Log("test 1");
  if (!make_token("1 + 2 * (3 - 4) / 5")) {
    panic("test 1: make_token failed");
  }
  Log("test 1: make_token succeeded");
  Log("test 2");
  if (!make_token("11")) {
    panic("test 2: make_token failed");
  }
  Log("test 2: make_token succeeded");
  Log("test 3");
  if (!make_token("1++1")) {
    panic("test 3: make_token failed");
  }
  Log("test 3: make_token succeeded");
  Log("test 4");
  if (!make_token("(1++1)")) {
    panic("test 4: make_token failed");
  }
  Log("test 4: make_token succeeded");
  Log("test 5");
  if (!make_token("+   +")) {
    panic("test 5: make_token failed");
  }
  Log("test 5: make_token succeeded");
  Log("test 6");
  if (!make_token("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789")) {
    panic("test 6: make_token failed");
  }
  Log("test 6: make_token succeeded");
  
  bool legal;
  Log("test 1");
  make_token("1+2*(3-4)/5");
  if (!check_parentheses(4, 8, &legal)) {
    panic("test 1: check paran failed");
  }
  if (!legal) {
    panic("test 1: expression is not legal");
  }
  Log("test 1 passed");
  Log("test 2");
  make_token("1+2*(3-4)/5");
  if (check_parentheses(3, 8, &legal)) {
    panic("test 2: check paran failed");
  }
  if (!legal) {
    panic("test 2: expression is not legal");
  }
  Log("test 2 passed");
  Log("test 3");
  make_token("(4 + 3 * (2 - 1))");
  if (!check_parentheses(0, 10, &legal)) {
    panic("test 3: check paran failed");
  }
  if (!legal) {
    panic("test 3: expression is not legal");
  }
  Log("test 3 passed");
  Log("test 4");
  make_token("4 + 3 * (2 - 1)");
  if (check_parentheses(0, 8, &legal)) {
    panic("test 4: check paran failed");
  }
  if (!legal) {
    panic("test 4: expression is not legal");
  }
  Log("test 4 passed");
  Log("test 5");
  make_token("(4 + 3)) * ((2 - 1)");
  if (check_parentheses(0, 12, &legal)) {
    panic("test 5: check paran failed");
  }
  if (legal) {
    panic("test 5: expression is not legal");
  }
  Log("test 5 passed");
  Log("test 6");
  make_token("(4 + 3) * (2 - 1)");
  if (check_parentheses(0, 11, &legal)) {
    panic("test 6: check paran failed");
  }
  if (!legal) {
    panic("test 6: expression is not legal");
  }
  Log("test 6 passed");
  
  bool success;
  word_t result;
  
  
  
  Log("test 2");
  result = expr("  (384087243u)  + 48143431u  \n", &success);
  if (!success) {
    panic("test 2: evaluate failed");
  }
  Log("test 2 passed = %" PRIu32 "\n", result);
  
  bool success;
  word_t result;

  Log("test 1");
  result = expr(" 0x1a2f \n", &success);
  if (!success) {
    panic("test 1: evaluate failed");
  }
  Log("test 1 passed = %" PRIu32 "\n", result);
  

  Log("test 2");
  result = expr("$$0\n", &success);
  if (!success) {
    panic("test 2: evaluate failed");
  }
  Log("test 2 passed = %" PRIu32 "\n", result);

  Log("test 3");
  result = expr("*0x80000000\n", &success);
  if (!success) {
    panic("test 3: evaluate failed");
  }
  Log("test 3 passed = %" PRIu32 "\n", result);

  Log("test 4");
  result = expr("*(0x80000000)\n", &success);
  if (!success) {
    panic("test 4: evaluate failed");
  }
  Log("test 4 passed = %" PRIu32 "\n", result);

  Log("test 5");
  result = expr("*(0x80000000) == *0x80000000\n", &success);
  if (!success) {
    panic("test 5: evaluate failed");
  }
  Log("test 5 passed = %" PRIu32 "\n", result);

  Log("test 6");
  result = expr("($s6==$s6)\n", &success);
  if (!success) {
    panic("test 6: evaluate failed");
  }
  Log("test 6 passed = %" PRIu32 "\n", result);

  Log("test 7");
  result = expr("(  *  (  0x80000000  ) == *0x80000000)&&($s6==$s6)\n", &success);
  if (!success) {
    panic("test 7: evaluate failed");
  }
  Log("test 7 passed = %" PRIu32 "\n", result);

  Log("test 8");
  result = expr("1&&0\n", &success);
  if (!success) {
    panic("test 8: evaluate failed");
  }
  Log("test 8 passed = %" PRIu32 "\n", result);
*/
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //    i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        int valid_num;
        switch (rules[i].token_type) {
          case TK_NOTYPE: break;  // do nothing for spaces
          case '+': 
            //Log("Token: +, nr_token = %d", nr_token);
            tokens[nr_token++].type = '+';
            break;
          case '-':
            //Log("Token: -, nr_token = %d", nr_token);
            tokens[nr_token++].type = '-';
            break;
          case '*':
            //Log("Token: *, nr_token = %d", nr_token);
            tokens[nr_token++].type = '*';
            break;
          case '/':
            //Log("Token: /, nr_token = %d", nr_token);
            tokens[nr_token++].type = '/';
            break;
          case '(':
            //Log("Token: (, nr_token = %d", nr_token);
            tokens[nr_token++].type = '(';
            break;
          case ')':
            //Log("Token: ), nr_token = %d", nr_token);
            tokens[nr_token++].type = ')';
            break;
          case TK_DEC:
            tokens[nr_token].type = TK_DEC;
            valid_num = snprintf(tokens[nr_token].str, sizeof(tokens[nr_token].str), "%.*s", substr_len, substr_start);
            //Log("Token: %s, valid_num = %d, nr_token = %d", tokens[nr_token].str, valid_num, nr_token);
            nr_token += valid_num > 0 ? 1 : 0; // Only increment if valid_num is positive
            break;
          case TK_EQ:
            //Log("Token: ==, nr_token = %d", nr_token);
            tokens[nr_token++].type = TK_EQ;
            break;
          case TK_NEQ:
            //Log("Token: !=, nr_token = %d", nr_token);
            tokens[nr_token++].type = TK_NEQ;
            break;
          case TK_AND:
            //Log("Token: &&, nr_token = %d", nr_token);
            tokens[nr_token++].type = TK_AND;
            break;
          case TK_HEX:
            tokens[nr_token].type = TK_HEX;
            valid_num = snprintf(tokens[nr_token].str, sizeof(tokens[nr_token].str), "%.*s", substr_len, substr_start);
            //Log("Token: %s, valid_num = %d, nr_token = %d", tokens[nr_token].str, valid_num, nr_token);
            nr_token += valid_num > 0 ? 1 : 0; // Only increment if valid_num is positive
            break;
          case TK_REG: 
            tokens[nr_token].type = TK_REG;
            valid_num = snprintf(tokens[nr_token].str, sizeof(tokens[nr_token].str), "%.*s", substr_len, substr_start);
            //Log("Token: %s, valid_num = %d, nr_token = %d", tokens[nr_token].str, valid_num, nr_token);
            nr_token += valid_num > 0 ? 1 : 0; // Only increment if valid_num is positive
            break;
          default: break;  // handle other token types
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  for (int i = 0; i < (int)strlen(e); i++) {
    if (tokens[i].type != TK_NOTYPE) {
      tokens[i].type = TK_NOTYPE;
    }
  }
  if (!make_token(e)) {
    Log("make_token failed for expression: %s", e);
    *success = false;
    return 0;
  }
  for (int i = 0; i < nr_token; i ++) {
    if (tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '+' 
                                         || tokens[i - 1].type == '-' 
                                         || tokens[i - 1].type == '*' 
                                         || tokens[i - 1].type == '/' 
                                         || tokens[i - 1].type == '('
                                         || tokens[i - 1].type == TK_EQ 
                                         || tokens[i - 1].type == TK_NEQ 
                                         || tokens[i - 1].type == TK_AND) ) {
      tokens[i].type = TK_DEREF;
    }
  }
  /* TODO: Insert codes to evaluate the expression. */ 
  return eval(0, (int)strlen(e) - 1, success);
}

word_t eval(int p, int q, bool *eval_success) {
  for (int i = p; i <= q; i++) {
    //Log("eval: tokens[%d] = %d", i, tokens[i].type);
    if (tokens[i].type != TK_NOTYPE && tokens[i].type != 0) {
      p = i;  // Find the first non-NOTYPE token
      break;
    }
  }
  for (int i = q; i >= p; i--) {
    //Log("eval: tokens[%d] = %d", i, tokens[i].type);
    if (tokens[i].type != TK_NOTYPE && tokens[i].type != 0) {
      q = i;  // Find the last non-NOTYPE token
      break;
    }
  }
  //Log("eval: p = %d, q = %d", p, q);
  bool paran_legal;
  int balance;
  bool in_paran;
  if (p > q) {
    /* Bad expression */
    //Log("Bad expression: p = %d, q = %d", p, q);
    *eval_success = false;
    return 0;
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if (tokens[p].type == TK_DEC) {
      //Log("Single token at p = %d: %s", p, tokens[p].str);
      *eval_success = true;
      return (word_t)atoi(tokens[p].str);
    }
    else if (tokens[p].type == TK_HEX) {
      //Log("Single token at p = %d: %s", p, tokens[p].str);
      *eval_success = true;
      char *end;
      return (word_t)strtoul(tokens[p].str, &end, 0);
    }
    else if (tokens[p].type == TK_REG) {
      //Log("Single token at p = %d: %s", p, tokens[p].str);
      *eval_success = true;
      bool success = false;
      word_t value = isa_reg_str2val(tokens[p].str+1, &success);
      if (!success) {
        //Log("Invalid register name at p = %d: %s", p, tokens[p].str);
        *eval_success = false; // Invalid register name
        return 0;
      }
      return value;
    }
    else {
      //Log("Invalid token type at p = %d: %c", p, tokens[p].type);
      *eval_success = false;
      return 0; // Invalid token type
    }
  }
  else if (check_parentheses(p, q, &paran_legal) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    int act_p = 0;
    int act_q = 0;
    for (int i = p; i <= q; i++) {
      if (tokens[i].type == '(') {
        act_p = i;
        break;
      }
    }
    for (int i = q; i >= p; i--) {
      if (tokens[i].type == ')') {
        act_q = i;
        break;
      }
    }

    //Log("Expression is surrounded by parentheses from %d to %d", act_p, act_q);
    return eval(act_p + 1, act_q - 1, eval_success);
  }
  else {
    if (paran_legal == false) {
      //Log("Unmatched parentheses from %d to %d", p, q);
      *eval_success = false; // Unmatched parentheses
      return 0;
    }
    //Log("Evaluating expression from %d to %d", p, q);
    *eval_success = true;
    //int curr = 0;
    int op = -1;
    int op_level = -1;
    balance = 0;
    in_paran = false;
    for (int i = p; i <= q; i++) {
      //Log("Checking token at position %d: %c", i, tokens[i].type);
      if (tokens[i].type == TK_DEC) {
        //Log("Found number at position %d: %s", i, tokens[i].str);
      }
      if (tokens[i].type == '(') {
        balance++;
        if (balance == 1) {
          in_paran = true; // Entering parentheses
        }
      }
      else if (tokens[i].type == ')') {
        balance--;
        if (balance == 0) {
          in_paran = false; // Exiting parentheses
        }
        else if (balance < 0) {
          //Log("Unmatched closing parenthesis at position %d", i);
          *eval_success = false; // Unmatched closing parenthesis
          return 0;
        }
      }
      else if (tokens[i].type == '+' || tokens[i].type == '-') {
        //Log("Checking operator at position %d: %c", i, tokens[i].type);
        if (in_paran) {
          continue; // Skip operators inside parentheses
        }
        //Log("Checking operator at position %d: %c", i, tokens[i].type);
        if (op_level < 3) {
          op = i;
          op_level = 2;
        }
      }
      else if (tokens[i].type == '*' || tokens[i].type == '/') {
        if (in_paran) {
          continue; // Skip operators inside parentheses
        }
        if (op_level < 2) {
          op = i;
          op_level = 1;
        }
      }
      else if (tokens[i].type == TK_EQ || tokens[i].type == TK_NEQ || tokens[i].type == TK_AND) {
        if (in_paran) {
          continue; // Skip operators inside parentheses
        }
        op = i;
        op_level = 3;
        if (tokens[i].type == TK_EQ) {
          //Log("Found equality operator at position %d: %c, op_level=%d", i, tokens[i].type, op_level);
        }
        else if (tokens[i].type == TK_NEQ) {
          //Log("Found inequality operator at position %d: %c, op_level=%d", i, tokens[i].type, op_level);
        }
        else if (tokens[i].type == TK_AND) {
          //Log("Found logical AND operator at position %d: %c, op_level=%d", i, tokens[i].type, op_level);
        }
      }
      else if (tokens[i].type == TK_DEREF) {
        if (in_paran) {
          continue; // Skip dereference operator inside parentheses
        }
        if (op_level < 1) {
          op = i;
          op_level = 0; // Dereference operator has the lowest precedence
        }
      }
    }
    //Log("Final op = %d, op_level = %d", op, op_level);
    if (op < 0) {
      char *end;
      return (word_t)strtol(tokens[p].str, &end, 10);
    }
    //Log("eval: p = %d, op - 1 = %d, op_level = %d", p, op-1, op_level);
    word_t val1 = eval(p, op - 1, eval_success);
    //Log("eval: op + 1 = %d, q = %d, val1 = %" PRIu32, op, q, val1);
    word_t val2 = eval(op + 1, q, eval_success);
    //Log("eval: p = %d, q = %d, op = %d, val1 = %" PRIu32 ", val2 = %" PRIu32 ", op_type = %c", p, q, op, val1, val2, tokens[op].type);
    int op_type = tokens[op].type;
    switch (op_type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': 
        if (val2 == 0) {
          //Log("Division by zero at position %d", op);
          *eval_success = false; // Division by zero
          return 0;
        }
        return (word_t)(val1 / val2);
      case TK_EQ: 
        //Log("Checking equality: %" PRIu32 " == %" PRIu32, val1, val2);
        return val1 == val2;
      case TK_NEQ: return val1 != val2;
      case TK_AND: 
        //Log("Checking logical AND: %" PRIu32 " && %" PRIu32, val1, val2);
        return val1 && val2;
      case TK_DEREF: 
        return vaddr_read(val2, 4); // Dereference the address in val2
      default: assert(0);
    }
  }
}

static bool check_parentheses(int p, int q, bool *legal) {
  //Log("check parentheses from %d to %d", p, q);
  //Log("tokens[p].type = %c, tokens[q].type = %c", tokens[p].type, tokens[q].type);
  bool check = true;
  *legal = true;
  if (tokens[p].type == ')') {  // If the first token is a closing parenthesis
    //Log("Unmatched closing parenthesis at position %d", p);
    legal = false;
    return false;
  }
  else if (tokens[p].type != '(') {  // If the first token is not an opening parenthesis
    //Log("Expression does not start with an opening parenthesis at position %d, %c", p, tokens[p].type);
    check = false;
  }
  //Log("check = %d", check);
  //Log("legal = %d", *legal);
  int balance = 0;
  bool out_paran = false; // Flag to check if we are outside parentheses
  for (int i = p; i <= q; i++) {
    if (tokens[i].type == '(') {
      balance++;
    } else if (tokens[i].type == ')') {
      balance--;
      if (balance < 0) {
        //Log("Unmatched closing parentheses at position %d, balance = %d", i, balance);
        *legal = false;
        return false; // Unmatched closing parenthesis
      }
      else if (balance == 0) {
        //Log("outside parentheses at position %d", i);
        //Log("check = %d", check);
        out_paran = true; // Outside parentheses
      }
    }
    else if (tokens[i].type != TK_NOTYPE && out_paran) {
      check = false; // If we encounter a non-NOTYPE token outside parentheses
    }
    
  }
  if (balance != 0) {
    //Log("Unmatched opening parentheses from %d to %d, balance = %d", p, q, balance);
    *legal = false; // Unmatched parentheses
    return false; // Unmatched opening parentheses
  }
  //Log("check = %d", check);
  //Log("legal = %d", *legal);
  return check; // All parentheses matched
}