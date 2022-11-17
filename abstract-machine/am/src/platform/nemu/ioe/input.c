#include <am.h>
#include <nemu.h>
#include <klib.h>
#define KEYDOWN_MASK 0x8000
#define KEY_QUEUE_LEN 1024
//static int keymap[256] = {};
//static int key_queue[KEY_QUEUE_LEN] = {};

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  //printf("yhy-%d\n",inl(KBD_ADDR));
  //int k=
  //kbd->keydown = (inl(KBD_ADDR) & KEYDOWN_MASK ? true : false);
  kbd->keydown=1;
  //kbd->keycode = inl(KBD_ADDR) & ~KEYDOWN_MASK;
  static int i=0;
  kbd->keycode=i;
  i++;
}
