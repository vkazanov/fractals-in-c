#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "draw.h"

#define WIDTH 50
#define HEIGHT 100

/*
  TODO: See the link below for further examples:
  https://github.com/skeeto/sort-circle/blob/master/sort.c

  TODO: extract a library of simple tools
  TODO: colorful ppms
  TODO: Error handling
  TODO: basic arg parsing
*/

#define XY_TO_BYTE(x, y)                        \
    ((y) * WIDTH + (x))

static void pbm_write(const uint8_t *buf, FILE *f)
{
    fprintf(f, "P1\n%d %d\n", WIDTH, HEIGHT);
    for (uint32_t y = 0; y < HEIGHT; ++y) {
        for (uint32_t x = 0; x < WIDTH; ++x) {
            fprintf(f, "%d", buf[XY_TO_BYTE(x, y)]);
        }
        fprintf(f, "\n");
    }

    fflush(f);
}


static void pbm_set(uint8_t *buf, uint32_t x, uint32_t y, uint8_t val)
{
    assert(x < WIDTH);
    assert(y < HEIGHT);

    buf[XY_TO_BYTE(x, y)] = val;
}

static void pbm_dot(uint8_t *buf, uint32_t x, uint32_t y)
{
    assert(x < WIDTH);
    assert(y < HEIGHT);

    pbm_set(buf, x, y, 1);
}

static int pbm_get(uint8_t *buf, int x, int y)
{
    assert(x < WIDTH);
    assert(y < HEIGHT);

    return buf[XY_TO_BYTE(x, y)];
}

static void pbm_line(uint8_t *buf,
                     uint32_t x_begin, uint32_t y_begin,
                     uint32_t x_end, uint32_t y_end)
{
    assert(x_end < WIDTH);
    assert(y_end < HEIGHT);

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
        pbm_dot(buf, x0, y0);

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

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    /* a monochrome buffer */
    uint8_t buf[WIDTH * HEIGHT] = {0};

    /* diagonal line */
    for (uint32_t i = 0; i < 30; ++i) {
        pbm_set(buf, i, i, 1);
    }

    /* horizontal line */
    for (uint32_t i = 0; i < 30; ++i) {
        pbm_set(buf, i, 40, 1);
    }

    /* cross */
    pbm_line(buf, 5, 60, 45, 80);
    pbm_line(buf, 5, 80, 45, 60);

    /* frame */
    pbm_line(buf, 0, 0, 0, HEIGHT - 1);
    pbm_line(buf, WIDTH - 1, 0, WIDTH - 1, HEIGHT - 1);

    pbm_line(buf, 0, 0, WIDTH - 1, 0);
    pbm_line(buf, 0, HEIGHT - 1, WIDTH - 1, HEIGHT - 1);

    const char *filename = "output.pbm";
    FILE *f = fopen(filename, "w");
    pbm_write(buf, f);

    return EXIT_SUCCESS;
}
