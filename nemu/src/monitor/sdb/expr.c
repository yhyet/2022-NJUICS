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

#include <isa.h>
#include <stdbool.h>
//#include <reg.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
#include <memory/vaddr.h>
enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUMBER,TK_HEXNUM,TK_REGNAME,TK_NOTEQ,TK_AND,TK_DEREF,TK_NEG,

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
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"-",'-'},
  {"\\*",'*'},
  {"/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"0x[0-9,a-f]+",TK_HEXNUM},
  {"[0-9]+",TK_NUMBER},
  {"\\$[$rsgta0-9]{2,3}",TK_REGNAME},
  {"!=",TK_NOTEQ},
  {"&&",TK_AND},


  //{"^\\+?[1-9][0-9]*$",TK_NUMBER}
  //{"^[0-9]|[1-9][0-9]+$",TK_NUMBER},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  //printf("1\n");
  int i;
  char error_msg[128];
  int ret;
//put zhengze in the re--yhy
  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
        //printf("pmatch.rm_so=%d\n",pmatch.rm_so);
        if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        //printf("rmeo=%d\n",substr_len);
        //printf("%c %c\n",e[position],e[position+1]);
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        //printf("substr_len=%d\n",substr_len);
        switch (rules[i].token_type) {
	  case TK_NOTYPE: /*printf("space");*/break;
	  case '+': tokens[nr_token].type='+';nr_token++;break;
	  case '-': tokens[nr_token].type='-';nr_token++;break;
	  case '*': tokens[nr_token].type='*';nr_token++;break;
	  case '/': tokens[nr_token].type='/';nr_token++;break;
	  case '(': tokens[nr_token].type='(';nr_token++;break;
	  case ')': tokens[nr_token].type=')';nr_token++;break;
    case TK_REGNAME: tokens[nr_token].type=TK_REGNAME;strncpy(tokens[nr_token].str,e+position-substr_len+1,substr_len-1);tokens[nr_token].str[substr_len-1]='\0';nr_token++;break;
    case TK_HEXNUM:	tokens[nr_token].type=TK_HEXNUM;strncpy(tokens[nr_token].str,e+position-substr_len+2,substr_len-2);tokens[nr_token].str[substr_len-2]='\0';printf("hexstr=%s\n",tokens[nr_token].str);nr_token++;break;
	  case TK_NUMBER: tokens[nr_token].type=TK_NUMBER;strncpy(tokens[nr_token].str,e+position-substr_len,substr_len);tokens[nr_token].str[substr_len]='\0';nr_token++;break;
	  case TK_EQ: tokens[nr_token].type=TK_EQ;nr_token++;break;
    case TK_NOTEQ: tokens[nr_token].type=TK_NOTEQ;nr_token++;break;
    case TK_AND: tokens[nr_token].type=TK_AND;nr_token++;break;
          default: assert(0);
        }
        assert(e!=NULL);
        
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
static bool check_parentheses(int p,int q);

static int op_find(int p,int q);

static word_t eval(int p,int q);

word_t isa_reg_str2val(const char *s,bool *success);

static bool certain_type(int t);


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  for (int i = 0; i < nr_token; i++)
  {
    if (tokens[i].type=='-' && i==0)
    {
      tokens[i].type=TK_NEG;
      //fuhao
    }
    else if (tokens[i].type=='*' && i==0)
    {
      tokens[i].type=TK_DEREF;
    }
      //jieyinyong
    
    else if (tokens[i].type=='-' && certain_type(tokens[i-1].type))
    {
      tokens[i].type=TK_NEG;
      //fuhao
    }
    else if (tokens[i].type=='*' && certain_type(tokens[i-1].type))
    {
      tokens[i].type=TK_DEREF;
    }
      //jieyinyong
  
  }
  printf("t0=%d\n",tokens[0].type);
  //*success=true;
  return eval(0,nr_token-1);
}

static bool certain_type(int t){
  //printf("%c\n",t);
  //fuhao--true
  switch (t)
  {
  case '+':return true;
  case TK_EQ:return true;
  case '-':return true; 
  case '*':return true; 
  case '/':return true; 
  case '(':return true; 
  case ')':return false; 
  case TK_REGNAME:return false; 
  case TK_HEXNUM:return false; 
  case TK_NUMBER:return false; 
  case TK_NOTEQ:return true; 
  case TK_AND:return true; 
  case TK_DEREF:return true;
  case TK_NEG:return true;
  default:
    assert(0);
    break;
  }
} 


static bool check_parentheses(int p,int q){
  //printf("p,q,token3=%d,%d,%c\n",p,q,tokens[3].type);
  if (tokens[p].type=='(' && tokens[q].type==')'){
    int num=0;
    for(int i=p+1;i<=q-1;i++){
      
      if(tokens[i].type=='(')
        num++;
      else if (tokens[i].type==')'){
        num=num-1;
        //printf("yhy");
        if (num<0)
        return 0;}
      //printf("number=%d\n",num);
    }
    if (num==0)
      return 1;
  }
  //assert(0);
  return 0;
}
/*
static int op_find(int p,int q){
  int parentheses_right=0;
  int two_flag=0;
  int mem=0;
  for(int i=q;i>=p;i--){
    if(tokens[i].type==')')
        parentheses_right++;
    else if (tokens[i].type=='(')
      parentheses_right--;
    else if (tokens[i].type=='+' ||tokens[i].type=='-'||tokens[i].type=='*'||tokens[i].type=='/'){
      if (parentheses_right>0)
        continue;
      else{
        if(tokens[i].type=='+'||tokens[i].type=='-')
          return i;
        else{ 
          if(two_flag==0){
          two_flag=1;
          mem=i;}
        }
        }
      }
    }
    if(mem!=0)
      return mem;
    assert(0);
  }*/
static int priority(int p){
  //printf("xx");
  //printf("%d\n",p);
  switch (tokens[p].type)
  {  
  case '+':return 4;
  case TK_EQ:return 7;
  case '-':return 4; 
  case '*':return 3; 
  case '/':return 3; 
  case TK_NOTEQ:return 7; 
  case TK_AND:return 11;
  case TK_DEREF:return 2;
  case TK_NEG:return 2; 
  default:return 0;
    break;
  }
}

static int op_find(int p,int q){
  
  int place[1000]={0};
  int j=0;
  int parentheses=0;
  for (int i = q; i >= p; i--)
  {
    if (tokens[i].type=='(') {parentheses--;continue;}
    if (tokens[i].type==')') {parentheses++;continue;}
    if (parentheses<0) assert(0);
    
    if(parentheses==0){
      if (tokens[i].type!=TK_REGNAME && tokens[i].type!=TK_HEXNUM && tokens[i].type!=TK_NUMBER)
      {
        place[j]=i;
        j++; 
    }
    }
  }
  int max_priority=0;
  int max_token=0;
  int max_place=0;
  for (int i = 0; i < j; i++)
  {
      if (priority(place[i])>max_priority){ 
      max_place=i;
      max_token=place[i];
      max_priority=priority(max_token);}
  }
  printf("maxplace=%d\n",max_place);
  if (max_priority==2){
    while (priority(place[max_place])==2)
    {
      max_place++;
      max_token=place[max_place];
      if (max_place==j-1) break;
    }
    
  }
  printf("%d\n",max_token);

    return max_token;
}


static word_t eval(int p,int q){
  //printf("nrtoken=%d\n",nr_token);
  //printf("01\n");
  //printf("p,q=%d,%d\n",p,q);
  if (p>q){
    printf("error--p,q=%d,%d\n",p,q);
    assert(0);
  }
  else if (p==q){
    word_t N=0;
    bool* success=0;
    switch (tokens[p].type)
    {
    case TK_NUMBER: /*printf("num=%s\n",tokens[p].str);*/sscanf(tokens[p].str,"%d",&N);break;
    case TK_HEXNUM: /*printf("hex=%s\n",tokens[p].str);*/sscanf(tokens[p].str,"%x",&N);break;
    case TK_REGNAME: /*printf("reg=%s\n",tokens[p].str);*/N=isa_reg_str2val(tokens[p].str,success);break;
    default:
      break;
    }
    //printf("%u\n",N);
    return N;
  }
  else if (check_parentheses(p,q)==1){
    return eval(p+1,q-1);
  }
  else{
    
    //printf("p=%d,q=%d\n",p,q);
    int op=op_find(p,q);
    if (tokens[op].type==TK_DEREF){
      //printf("point\n");
      word_t val=eval(op+1,q);
      return vaddr_read(val,4);
      //return *val;
    }
    else if (tokens[op].type==TK_NEG){
      int val=eval(op+1,q);
      return -val;
    }
    else{
      //printf("op=%d\n",op);
      word_t val1=eval(p,op-1);
      word_t val2=eval(op+1,q);

      switch (tokens[op].type)
      {
      case '+':return val1+val2;
      case '-':return val1-val2;
      case '*':return val1*val2;
      case '/':return val1/val2;
      case TK_EQ:return val1==val2;
      case TK_NOTEQ:return val1!=val2;
      case TK_AND:return val1&&val2;
      default:assert(0);
    }
    }
  }
}
