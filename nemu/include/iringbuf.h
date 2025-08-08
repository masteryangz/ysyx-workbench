#ifndef IRINGBUF_H
#define IRINGBUF_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define RINGBUF_STRING_SIZE 96
#define RINGBUF_SIZE 16

typedef struct {
    char buf[RINGBUF_SIZE][RINGBUF_STRING_SIZE];
    size_t head;
    size_t tail;
} ringbuf_t;

extern ringbuf_t iringbuf;

// Initialize
void ringbuf_init(ringbuf_t *rb);

// Push one byte
void ringbuf_push(ringbuf_t *rb, uint32_t pc, uint8_t *code, int nbyte);

void print_ringbuf(ringbuf_t *rb);

#endif