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

#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;

static uint32_t sbuf_size = CONFIG_SB_SIZE;
static uint32_t count = 0;   // 已用大小
static uint32_t head = 0;    // 读指针
//static uint32_t tail = 0;    // 写指针

static uint32_t freq, channels, samples;

static void sdl_audio_callback(void *userdata, Uint8 *stream, int len) {
  for (int i = 0; i < len; i++) {
    if (count == 0) {
      stream[i] = 0;  // 缓冲区空，输出静音
    } else {
      stream[i] = sbuf[head];
      head = (head + 1) % sbuf_size;
      count--;
    }
  }
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
  Log("audio io at offset 0x%x, len = %d, is_write = %d", offset, len, is_write);
  int index = offset / 4;

  if (is_write) {
    switch (index) {
      case reg_freq:     freq     = audio_base[reg_freq];     break;
      case reg_channels: channels = audio_base[reg_channels]; break;
      case reg_samples:  samples  = audio_base[reg_samples];  break;
      case reg_init: {
        if (audio_base[reg_init]) {
          SDL_AudioSpec s;
          s.freq     = freq;
          s.channels = channels;
          s.samples  = samples;
          s.format   = AUDIO_S16SYS;
          s.callback = sdl_audio_callback;
          s.userdata = NULL;
          Log("audio init: freq = %d, channels = %d, samples = %d", freq, channels, samples);
          count = 0;
          int ret = SDL_InitSubSystem(SDL_INIT_AUDIO);
          if (ret == 0) {
            SDL_OpenAudio(&s, NULL);
            Log("audio playing");
            SDL_PauseAudio(0);
          }
        }
        break;
      }
      default: break;
    }
  } else {
    switch (index) {
      case reg_sbuf_size: audio_base[reg_sbuf_size] = sbuf_size; break;
      case reg_count:     audio_base[reg_count]     = count;     break;
      default: break;
    }
  }
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif

  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);

  audio_base[reg_sbuf_size] = sbuf_size;
  audio_base[reg_count]     = 0;
}
