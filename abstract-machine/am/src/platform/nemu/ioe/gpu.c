#include <am.h>
#include <nemu.h>
#include <klib.h>
#define SYNC_ADDR (VGACTL_ADDR + 4)
#define W 400
#define H 300

void __am_gpu_init() {
  int i;
  int w=W;
  int h=H;
  uint32_t *fb=(uint32_t *)(uintptr_t)FB_ADDR;
  for ( i = 0; i < w*h; i++)
  {
    fb[i]=i;
  }
  outl(SYNC_ADDR,1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = W, .height = H,
    .vmemsz = W*H*sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (!ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
  int i = 0;
  int j = 0;
  //ctl->h = H;
  //ctl->w = W;
  //printf("ctl->h=%d",ctl->h);
  for(i = 0;i <ctl->h;i++){
    for(j = 0;j <ctl->w;j++){
        outl((uintptr_t)FB_ADDR +ctl->x*4+ctl->y*W*4+ i*W*4 + j*4,*(uint32_t *)(ctl->pixels));
        (ctl->pixels)+=4;
        
    }
  }
  //outl(FB_ADDR,1);
  //printf("%d,%d\n",ctl->h,ctl->w);
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
