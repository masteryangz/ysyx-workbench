#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  int w = io_read(AM_GPU_CONFIG).width / N;
  int h = io_read(AM_GPU_CONFIG).height / N;
  int block_size = w * h;
  assert((uint32_t)block_size <= LENGTH(color_buf));
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  /*
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 0, .height = 0,
    .vmemsz = 0
  };
  */
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = io_read(AM_GPU_CONFIG).width / N, .height = io_read(AM_GPU_CONFIG).height / N,
    .vmemsz = io_read(AM_GPU_CONFIG).width / N * io_read(AM_GPU_CONFIG).height / N * sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  ctl->pixels = (void *)((uintptr_t)ctl->pixels + (io_read(AM_GPU_CONFIG).width / N) * ctl->y * sizeof(uint32_t) + ctl->x * sizeof(uint32_t));
  int w = io_read(AM_GPU_CONFIG).width / N;
  int h = io_read(AM_GPU_CONFIG).height / N;
  int block_size = w * h;
  assert((uint32_t)(ctl->w * ctl->h) <= LENGTH(color_buf));
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
