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

void pbm_dot_safe(pbm_t *pbm, int x, int y)
{
    if (x >= 0 && y >= 0 && x < pbm->width && y < pbm->height)
        pbm_dot(pbm, (uint32_t)x, (uint32_t)y);
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

ppm_t *ppm_create(uint32_t width, uint32_t height)
{
    ppm_t *ppm = calloc(1, sizeof(ppm_t));
    assert(ppm);

    *ppm = (typeof(*ppm)) {
        .width = width,
        .height = height,
        .buf = calloc(width * height, sizeof(ppm_color_t))
    };
    assert(ppm->buf);

    return ppm;
}

void ppm_destroy(ppm_t *ppm)
{
    free(ppm->buf);
    free(ppm);
}

void ppm_write(ppm_t *ppm, FILE *f)
{
#define MAX_COLOR 255

    fprintf(f, "P3\n%d %d\n%d\n", ppm->width, ppm->height, 255);

    for (uint32_t y = 0; y < ppm->height; ++y) {
        for (uint32_t x = 0; x < ppm->width; ++x) {
            ppm_color_t color = ppm->buf[XY_TO_BYTE(x, y, ppm->width)];
            fprintf(f, "%d %d %d\n", color.red, color.green, color.blue);
        }
    }

    fflush(f);

#undef MAX_COLOR
}

void ppm_dot(ppm_t *ppm, uint32_t x, uint32_t y, ppm_color_t color)
{
    assert(x < ppm->width);
    assert(y < ppm->height);

    ppm->buf[XY_TO_BYTE(x, y, ppm->width)] = color;
}
