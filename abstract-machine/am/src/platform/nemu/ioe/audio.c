#include <am.h>
#include <nemu.h>
#include <string.h>
#include <stdio.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

void __am_audio_init() {
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
  cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR);
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  outl(AUDIO_FREQ_ADDR, ctrl->freq);
  outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
  outl(AUDIO_SAMPLES_ADDR, ctrl->samples);
  outl(AUDIO_INIT_ADDR, 1);  // 写 1 表示初始化
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = inl(AUDIO_COUNT_ADDR);
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
  printf("audio play: start = %d, end = %d\n", ctl->buf.start, ctl->buf.end);
  uint8_t *src = (uint8_t *)ctl->buf.start;
  int len = ctl->buf.end - ctl->buf.start;

  int bufsize = inl(AUDIO_SBUF_SIZE_ADDR);

  while (len > 0) {
    int count = inl(AUDIO_COUNT_ADDR);
    int free_space = bufsize - count;
    if (free_space == 0) continue;  // 忙等，直到有空闲空间

    int chunk = (len < free_space ? len : free_space);

    // 注意这里直接写到流缓冲区 (CONFIG_SB_ADDR)
    int head = count % bufsize;
    int first_part = (head + chunk <= bufsize ? chunk : bufsize - head);

    // 写前半部分
    memcpy((void *)(uintptr_t)(AUDIO_SBUF_ADDR + head), src, first_part);

    // 如果跨越了缓冲区末尾，写剩下的部分
    if (first_part < chunk) {
      memcpy((void *)(uintptr_t)(AUDIO_SBUF_ADDR), src + first_part, chunk - first_part);
    }

    src += chunk;
    len -= chunk;
  }
}
