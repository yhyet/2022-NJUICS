/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
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

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char exprssion[32];
  int value;
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
/*
static WP* new_wp(char* exp){
  if(free_==NULL) assert(0);
  WP* new_head;
  new_head=free_;
  free_=free_->next;
  new_head->next=NULL;
  strcpy(new_head->exprssion,exp);
  bool* success=0;
  new_head->value=expr(exp,success);
  if (head==NULL){
    head=new_head;
  }
  else{
    WP* q=head;
    while (q->next!=NULL)
    {
      q=q->next;
    }
    q->next=new_head;
  }
  return new_head;
}

static void free_wp(WP *wp){
  //wp->value=0;
  //menset(wp->exprssion,0,32);
  if(wp==head){
    head=head->next;
  }
  else{
    WP* p=head;
    while (p->next!=wp)
    {
      p=p->next;
    }
    p->next=wp->next;
  }
  wp->next=free_;
  free_=wp;
  return;
}

static bool scan_wp(char* wp){
  
}
*/