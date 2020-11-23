#include "draw.h"

#include <stdbool.h>
#include <assert.h>

#define XY_TO_BYTE(x, y, width)                 \
    ((y) * (width) + (x))

pbm_t *pbm_create(uint16_t width, uint16_t height)
{
    pbm_t *pbm = calloc(1, sizeof(pbm_t));
    assert(pbm);

    *pbm = (typeof(*pbm)) {
        .width = width,
        .height = height,
        .buf = calloc(1, (size_t)width * height)
    };
    assert(pbm->buf);

    return pbm;
}

void pbm_destroy(pbm_t *pbm)
{
    free(pbm->buf);
    free(pbm);
}

void pbm_write(pbm_t *pbm, FILE *f)
{
    fprintf(f, "P1\n%d %d\n", pbm->width, pbm->height);
    for (uint32_t y = 0; y < pbm->height; ++y) {
        for (uint32_t x = 0; x < pbm->width; ++x) {
            fprintf(f, "%d", pbm->buf[XY_TO_BYTE(x, y, pbm->width)]);
        }
        fprintf(f, "\n");
    }

    fflush(f);
}


void pbm_set(pbm_t *pbm, uint32_t x, uint32_t y, uint8_t val)
{
    assert(x < pbm->width);
    assert(y < pbm->height);

    pbm->buf[XY_TO_BYTE(x, y, pbm->width)] = val;
}

void pbm_dot(pbm_t *pbm, uint32_t x, uint32_t y)
{
    assert(x < pbm->width);
    assert(y < pbm->height);

    pbm_set(pbm, x, y, 1);
}

int pbm_get(pbm_t *pbm, int x, int y)
{
    assert(x < pbm->width);
    assert(y < pbm->height);

    return pbm->buf[XY_TO_BYTE(x, y, pbm->width)];
}

void pbm_line(pbm_t *pbm,
              uint32_t x_begin, uint32_t y_begin,
              uint32_t x_end, uint32_t y_end)
{
    assert(x_end < pbm->width);
    assert(y_end < pbm->height);

    int64_t x0 = x_begin, y0 = y_begin, x1 = x_end, y1 = y_end;

    /* Bresenham's line algorithm */

#define SIGN(left, right)                       \
    ((left) < (right) ? 1 : -1)
#define DIFF(left, right)                                       \
    ((left) < (right) ? (right) - (left) : (left) - (right))

    int dx = DIFF(x0, x1);
    int sx = SIGN(x0, x1);

    int dy = -DIFF(y0, y1);
    int sy = SIGN(y0, y1);

    int err = dx + dy;
    while(true) {
        pbm_dot(pbm, x0, y0);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }

#undef DIFF
#undef SIGN
}
