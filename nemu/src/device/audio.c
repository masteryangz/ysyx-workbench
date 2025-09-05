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

#define AUDIO_CTL_SIZE 24   // 6 * 4 bytes
#define SBUF_SIZE (64 * 1024) // 64KB

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;
static uint32_t sbuf_head = 0; // read index
static uint32_t sbuf_tail = 0; // write index
static SDL_AudioDeviceID dev = 0;
static SDL_AudioSpec want;

static SDL_mutex *audio_mutex = NULL;

static inline uint32_t sbuf_used() {
  if (sbuf_tail >= sbuf_head) return sbuf_tail - sbuf_head;
  return SBUF_SIZE - (sbuf_head - sbuf_tail);
}

static inline uint32_t sbuf_free() {
  return SBUF_SIZE - sbuf_used() - 1; // leave one byte free to distinguish full/empty
}

// SDL audio callback, called in SDL audio thread
static void audio_callback(void *userdata, Uint8 *stream, int len) {
  SDL_LockMutex(audio_mutex);
  int to_read = len;
  int outpos = 0;
  while (to_read > 0) {
    uint32_t used = sbuf_used();
    if (used == 0) {
      // 缓冲区空，填 0 (静音)
      memset(stream + outpos, 0, to_read);
      outpos += to_read;
      to_read = 0;
      break;
    }
    uint32_t chunk = used;
    if (sbuf_tail > sbuf_head) chunk = sbuf_tail - sbuf_head;
    // chunk 是连续可读大小
    uint32_t take = chunk < (uint32_t)to_read ? chunk : (uint32_t)to_read;
    memcpy(stream + outpos, sbuf + sbuf_head, take);
    sbuf_head = (sbuf_head + take) % SBUF_SIZE;
    outpos += take;
    to_read -= take;
  }
  // 更新 regs[5] = count (已使用字节数)
  audio_regs[5] = sbuf_used();
  SDL_UnlockMutex(audio_mutex);
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
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
}
