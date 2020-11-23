#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct pbm_t {
    uint16_t width;
    uint16_t height;
    uint8_t *buf;
} pbm_t;

pbm_t *pbm_create(uint16_t width, uint16_t height);

void pbm_destroy(pbm_t *pbm);

void pbm_write(pbm_t *pbm, FILE *f);

void pbm_set(pbm_t *pbm, uint32_t x, uint32_t y, uint8_t val);

void pbm_dot(pbm_t *pbm, uint32_t x, uint32_t y);

int pbm_get(pbm_t *pbm, int x, int y);

void pbm_line(pbm_t *pbm,
              uint32_t x_begin, uint32_t y_begin,
              uint32_t x_end, uint32_t y_end);

#endif /* DRAW_H */
