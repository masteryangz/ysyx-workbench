#include <iringbuf.h>
//#include "utils/disasm.c"

void ringbuf_init(ringbuf_t *rb) {
    //iringbuf.buf = NULL;
    rb->head = rb->tail = 0;
}

void ringbuf_push(ringbuf_t *rb, uint32_t pc, uint8_t *code, int nbyte) {
    
  // Clear previous arrow
  size_t prev = (rb->head + RINGBUF_SIZE - 1) % RINGBUF_SIZE;
  memcpy(rb->buf[prev], "    ", 4);

  char *line = rb->buf[rb->head];
  strcpy(line, "--> ");  // leading arrow

  char disas[64];
  char raw[32];
  int disassemble_full(char *str, int str_size, char *rawbuf, int rawbuf_size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble_full(disas, sizeof(disas), raw, sizeof(raw), pc, code, nbyte);

  snprintf(line + 4, RINGBUF_STRING_SIZE - 4, "0x%08x: %-30.30s %.30s", pc, disas, raw);

  rb->head = (rb->head + 1) % RINGBUF_SIZE;
  
}

void print_ringbuf(ringbuf_t *rb) {
    for (int i = 0; i < RINGBUF_SIZE; i++) {
        printf("%s\n", rb->buf[i]);
    }
}