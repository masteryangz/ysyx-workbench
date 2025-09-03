#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  //kbd->keydown = 0;
  //kbd->keycode = AM_KEY_NONE;
  uint32_t key = inl(KBD_ADDR);
  if (key == 0) {
    //printf("no key is pressed\n");
    kbd->keydown = false;
    kbd->keycode = AM_KEY_NONE;
  } else {
    printf("key = %x\n", key);
    kbd->keydown = (key & KEYDOWN_MASK) != 0;
    kbd->keycode = key & ~KEYDOWN_MASK;
  }
}
