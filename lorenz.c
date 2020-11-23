#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "draw.h"

/*
  TODO: See the link below for further examples:
  https://github.com/skeeto/sort-circle/blob/master/sort.c

  TODO: colorful ppms
  TODO: Error handling
  TODO: basic arg parsing
*/


int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    /* a monochrome buffer */
    pbm_t *pbm = pbm_create(EGA_WIDTH, EGA_HEIGHT);

    /* diagonal line */
    for (uint32_t i = 0; i < 30; ++i) {
        pbm_set(pbm, i, i, 1);
    }

    /* horizontal line */
    for (uint32_t i = 0; i < 30; ++i) {
        pbm_set(pbm, i, 40, 1);
    }

    /* cross */
    pbm_line(pbm, 5, 60, 45, 80);
    pbm_line(pbm, 5, 80, 45, 60);

    /* frame */
    pbm_line(pbm, 0, 0, 0, EGA_HEIGHT - 1);
    pbm_line(pbm, EGA_WIDTH - 1, 0, EGA_WIDTH - 1, EGA_HEIGHT - 1);

    pbm_line(pbm, 0, 0, EGA_WIDTH - 1, 0);
    pbm_line(pbm, 0, EGA_HEIGHT - 1, EGA_WIDTH - 1, EGA_HEIGHT - 1);

    const char *filename = "output.pbm";
    FILE *f = fopen(filename, "w");
    pbm_write(pbm, f);

    pbm_destroy(pbm);

    return EXIT_SUCCESS;
}
