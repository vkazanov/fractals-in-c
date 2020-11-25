#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "draw.h"


typedef void project_function_t(double x, double y, double z,
                                double col_scale, int col_offset,
                                double row_scale, int row_offset,
                                int *col, int *row);

void project_xz(double x, double y, double z,
                double col_scale, int col_offset,
                double row_scale, int row_offset,
                int *col, int *row)
{
    (void) y;
    *col = x * col_scale + col_offset;
    *row = z * row_scale + row_offset;
}

void project_yz(double x, double y, double z,
                double col_scale, int col_offset,
                double row_scale, int row_offset,
                int *col, int *row)
{
    (void) x;
    *col = y * col_scale + col_offset;
    *row = z * row_scale + row_offset;
}

void project_yx(double x, double y, double z,
                double col_scale, int col_offset,
                double row_scale, int row_offset,
                int *col, int *row)
{
    (void) z;
    *col = y * col_scale + col_offset;
    *row = x * row_scale + row_offset;
}

int main(int argc, char *argv[])
{
    /* Arguments */
    char *output_filename = "output.pbm";
    int col_offset = 0;
    int row_offset = 0;
    double scale = 1;
    project_function_t *project = project_xz;

    int c;
    while ((c = getopt(argc, argv, "123c:r:s:o:")) != -1) {
        char *endptr;
        switch (c) {
        case 'o':
            output_filename = optarg;
            break;
        case '1':
            project = project_xz;
            break;
        case '2':
            project = project_yz;
            break;
        case '3':
            project = project_yx;
            break;
        case 'c':
            col_offset = strtol(optarg, &endptr, 10);
            break;
        case 'r':
            row_offset = strtol(optarg, &endptr, 10);
            break;
        case 's':
            scale = strtod(optarg, &endptr);
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    /* Drawing plane */
    pbm_t *pbm = pbm_create(EGA_WIDTH, EGA_HEIGHT);

    /* Runge Kutta Integration  */
    double dt = 0.005;
    double dt2 = dt / 2;
    double third = 0.333333333;
    double x = 0, y = 1, z = 0;
    double xt, yt, zt;

    x = 0; y = 1; z = 0;

    int old_col;
    int old_row;
    project(x, y, z, scale, col_offset, scale, row_offset, &old_col, &old_row);

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

        /* Project the Lorenz attractor onto the plane */
        int col, row;
        project(x, y, z, scale, col_offset, scale, row_offset, &col, &row);
        pbm_line_safe(pbm, old_col, old_row, col, row);

        old_col = col;
        old_row = row;
    }

    /* Dump results */
    FILE *f = fopen(output_filename, "w");
    pbm_write(pbm, f);
    fclose(f);

    pbm_destroy(pbm);

    return EXIT_SUCCESS;
}
