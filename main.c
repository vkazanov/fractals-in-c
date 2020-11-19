#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "draw.h"

#define WIDTH 50
#define HEIGHT 100

/*
  TODO: See the link below for further examples:
  https://github.com/skeeto/sort-circle/blob/master/sort.c

  TODO: extract a library of simple tools
  TODO: colorful ppms

*/

#define XY_TO_BYTE(x, y)                        \
    (y) * WIDTH + (x)

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
    buf[XY_TO_BYTE(x, y)] = val;
}

static int pbm_get(uint8_t *buf, int x, int y)
{
    return buf[XY_TO_BYTE(x, y)];
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    /* a monochrome buffer */
    uint8_t buf[WIDTH * HEIGHT] = {0};

    /* diagonal */
    for (uint32_t i = 0; i < 30; ++i) {
        pbm_set(buf, i, i, 1);
    }

    /* horizontal line */
    for (uint32_t i = 0; i < 30; ++i) {
        pbm_set(buf, i, 40, 1);
    }

    const char *filename = "output.pbm";
    FILE *f = fopen(filename, "w");
    pbm_write(buf, f);

    return EXIT_SUCCESS;
}
