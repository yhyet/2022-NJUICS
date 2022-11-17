#include <am.h>
#include <nemu.h>
#include <klib.h>
#define KEYDOWN_MASK 0x8000
static int sec=1;
void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  //int k=
  //kbd->keydown = (inl(KBD_ADDR) & KEYDOWN_MASK ? true : false);
  while(io_read(AM_TIMER_UPTIME).us / 1000000 < sec){
    ;
  }
  printf("yhy-%d\n",inl(KBD_ADDR));
  sec++;
  kbd->keydown=1;
  //kbd->keycode = inl(KBD_ADDR) & ~KEYDOWN_MASK;
  //static int i=0;
  kbd->keycode=0;
  //i++;
}
