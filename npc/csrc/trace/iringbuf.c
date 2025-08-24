#include <trace/iringbuf.h>
#include <common.h>
int disassemble_full(char *str, int str_size, char *rawbuf, int rawbuf_size, uint64_t pc, uint8_t *code, int nbyte);
void ringbuf_init(ringbuf_t *rb) {
    for (size_t i = 0; i < RINGBUF_SIZE; i++) {
        memset(rb->buf[i], 0, RINGBUF_STRING_SIZE);
    }
    //iringbuf.buf = NULL;
    rb->head = rb->tail = 0;
}

void ringbuf_push(ringbuf_t *rb, uint32_t pc, uint8_t *code, int nbyte) {
  //Log("pc = 0x%08x", pc);
  // Clear previous arrow
  size_t prev = (rb->head + RINGBUF_SIZE - 1) % RINGBUF_SIZE;
  if (rb->buf[prev][0] != '\0') {
    memcpy(rb->buf[prev], "    ", 4);
  }
  //printf("rb->head = %zu, rb->tail = %zu", rb->head, rb->tail);
  char *line = rb->buf[rb->head];
  strcpy(line, "--> ");  // leading arrow

  char disas[64];
  char raw[32];
  //printf("ringbuf_push: pc = 0x%08x, code = 0x%02x%02x%02x%02x, nbyte = %d\n", pc, code[0], code[1], code[2], code[3], nbyte);
#ifdef CONFIG_ITRACE
  disassemble_full(disas, sizeof(disas), raw, sizeof(raw), pc, code, nbyte);
#endif

  snprintf(line + 4, RINGBUF_STRING_SIZE - 4, "0x%08x: %-30.30s %.30s", pc, disas, raw);

  rb->head = (rb->head + 1) % RINGBUF_SIZE;
  //printf("rb->head = %zu, rb->tail = %zu", rb->head, rb->tail);

}

void print_ringbuf(ringbuf_t *rb) {
    for (int i = 0; i < RINGBUF_SIZE; i++) {
        printf("%s\n", rb->buf[i]);
    }
}