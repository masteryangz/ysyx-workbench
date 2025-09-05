#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  /*
  int i;
  int w = inl(VGACTL_ADDR) >> 16;
  int h = inl(VGACTL_ADDR) & 0xffff;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
  */
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  /*
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 0, .height = 0,
    .vmemsz = 0
  };
  */
  uint32_t val = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = val >> 16, .height = val & 0xffff,
    .vmemsz =  val & 0xffff * (val >> 16) * sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  int width  = inl(VGACTL_ADDR) >> 16;   // 屏幕宽度
  int height = inl(VGACTL_ADDR) & 0xffff; // 屏幕高度

  for (int j = 0; j < ctl->h; j++) {
    int py = ctl->y + j;
    if (py < 0 || py >= height) continue;   // 防止越界
    for (int i = 0; i < ctl->w; i++) {
      int px = ctl->x + i;
      if (px < 0 || px >= width) continue; // 防止越界
      fb[py * width + px] = ((uint32_t *)ctl->pixels)[j * ctl->w + i];
    }
  }

  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}


void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
