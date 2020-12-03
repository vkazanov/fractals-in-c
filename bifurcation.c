#include <unistd.h>

#include "util.h"

int main(int argc, char *argv[])
{
    /* Arguments */
    char *output_filename = "output.ppm";
    float r = .95, x_init = 0.5;

    int c;
    while ((c = getopt(argc, argv, "o:r:x:")) != -1) {
        char *endptr;
        switch (c) {
        case 'o':
            output_filename = optarg;
            break;
        case 'r':
            r = strtod(optarg, &endptr);
            break;
        case 'x':
            x_init = strtod(optarg, &endptr);
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    ppm_t *ppm = ppm_create(EGA_WIDTH, EGA_HEIGHT);

    /* Population equation */
    float delta_r = 0.005;
    for (int col = 0; col < 639; col++) {
        float x = x_init;
        r += delta_r;
        for (int i = 0; i < 256; ++i) {
            x = r * x * (1 - x);
            if ((x > 1000000) || (x < -1000000))
                break;

            int row = 349 - (x * 350);
            if (i > 64 && row < 349 && row >= 0 && col >= 0 && col < 639) {
                ppm_dot_safe(ppm, col, row, PPM_WHITE);
            }
        }
    }

    /* Dump results */
    FILE *f = fopen(output_filename, "w");
    ppm_write(ppm, f);
    fclose(f);

    ppm_destroy(ppm);

    return 0;
}
