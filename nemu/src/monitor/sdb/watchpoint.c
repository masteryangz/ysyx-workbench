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
#include "sdb.h"
//#include <monitor/sdb/expr.h>
#include <monitor/sdb/watchpoint.h>

static WP wp_pool[NR_WP] = {};
//static WP *free_ = NULL;
WP *wp_head = NULL; // Head of the watchpoint linked list
WP *free_ = NULL; // Pointer to the first free watchpoint in the pool

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  wp_head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char *expr, word_t value) {
  if (free_ == NULL) {
    assert(0); // No free watchpoints available
  }
  
  WP *new_wp = free_;
  free_ = free_->next;
  new_wp->expr = strdup(expr); // Duplicate the expression string
  new_wp->value = value; // Set the initial value
  new_wp->next = wp_head;
  wp_head = new_wp;
  //Log("New watchpoint created: %s = 0x%08x", new_wp->expr, new_wp->value);
  //Log("wp_head : %s = 0x%08x", wp_head->expr, wp_head->value);
  return new_wp;
}

void free_wp(WP *wp) {
  if (wp == NULL) {
    return; // Nothing to free
  }

  // Remove wp from the linked list
  if (wp_head == wp) {
    wp_head = wp->next;
  } else {
    WP *prev = wp_head;
    while (prev != NULL && prev->next != wp) {
      prev = prev->next;
    }
    if (prev != NULL) {
      prev->next = wp->next;
    }
  }

  // Add wp back to the free list
  wp->next = free_;
  free_ = wp;
}

void set_value(WP *wp, word_t value) {
  wp->value = value;
  return;
}