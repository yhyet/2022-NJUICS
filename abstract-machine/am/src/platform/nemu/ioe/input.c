#include <am.h>
#include <nemu.h>
#include <klib.h>
#define KEYDOWN_MASK 0x8000
#define KEY_QUEUE_LEN 1024
static int key_queue[KEY_QUEUE_LEN] = {};
static int i=0;
//static int sec=1;
void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  //int k=
  //kbd->keydown = (inl(KBD_ADDR) & KEYDOWN_MASK ? true : false);
  //while(io_read(AM_TIMER_UPTIME).us / 1000000 < sec){
  //  ;
  //}
  //printf("yhy-%d\n",inl(KBD_ADDR));
  //sec++;
  key_queue[i]=inl(KBD_ADDR);
  kbd->keydown = (key_queue[i] & KEYDOWN_MASK)? false : true;
  kbd->keycode = key_queue[i] & ~KEYDOWN_MASK;
  i=(i+1)%KEY_QUEUE_LEN;
  //static int i=0;
  //kbd->keycode=0;
  //i++;
}
