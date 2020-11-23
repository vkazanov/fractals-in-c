#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "draw.h"

/*
  TODO: See the link below for further examples:
  https://github.com/skeeto/sort-circle/blob/master/sort.c

  TODO: error in the line drawing algorithm?
  TODO: Error handling
  TODO: basic arg parsing
*/


int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    /* Drawing planes */
    pbm_t *pbm_yz = pbm_create(EGA_WIDTH, EGA_HEIGHT);
    pbm_t *pbm_yx = pbm_create(EGA_WIDTH, EGA_HEIGHT);

    /* Runge Kutta Integration  */
    double dt = 0.005;
    double dt2 = dt / 2;
    double third = 0.333333333;
    double x = 0, y = 1, z = 0;
    double xt, yt, zt;

    /* int old_col = y; */
    /* int old_row = z; */
    x = 0; y = 1; z = 0;

    for (int i = 0; i < 8000; i++) {

        double d0_x, d0_y, d0_z;
        double d1_x, d1_y, d1_z;
        double d2_x, d2_y, d2_z;
        double d3_x, d3_y, d3_z;

        /* Integration itself */

        d0_x = 10 * (y - x) * dt2;
        d0_y = (-xt * zt + 28 * xt - yt) * dt2;
        d0_z = (xt * yt - 8 * zt / 3) * dt2;

        xt = x + d0_x;
        yt = y + d0_y;
        zt = z + d0_z;

        d1_x = (10 * (yt - xt)) * dt2;
        d1_y = (-xt * zt + 28 * xt - yt) * dt2;
        d1_z = (xt * yt - 8 * zt / 3) * dt2;

        xt = x + d1_x;
        yt = x + d1_x;
        zt = x + d1_x;

        d2_x = (10 * (yt - xt)) * dt2;
        d2_y = (-xt * zt + 28 * xt - yt) * dt2;
        d2_z = (xt * yt - 8 * zt / 3) * dt2;

        xt = x + d2_x;
        yt = x + d2_x;
        zt = x + d2_x;

        d3_x = (10 * (yt - xt)) * dt2;
        d3_y = (-xt * zt + 28 * xt - yt) * dt2;
        d3_z = (xt * yt - 8 * zt / 3) * dt2;

        x += (d0_x + d1_x + d1_x + d2_x + d3_x) * third;
        y += (d0_y + d1_y + d1_y + d2_y + d3_y) * third;
        z += (d0_z + d1_z + d1_z + d2_z + d3_z) * third;

        /* Project the Lorenz attract onto the YZ plane */
        int col = y * 2 + 320;
        int row = z * 1.5;
        pbm_dot_safe(pbm_yz, col, row);

        /* Project the Lorenz attract onto the YX plane */
        col = y * 2 + 320;
        row = x * 2 + 175;
        pbm_dot_safe(pbm_yx, col, row);
    }

    /* Dump results */
    FILE *f = fopen("lorenz_yz.pbm", "w");
    pbm_write(pbm_yz, f);
    fclose(f);

    f = fopen("lorenz_yx.pbm", "w");
    pbm_write(pbm_yx, f);
    fclose(f);

    pbm_destroy(pbm_yz);
    pbm_destroy(pbm_yx);

    return EXIT_SUCCESS;
}
