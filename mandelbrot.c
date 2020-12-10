#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

#define WIDTH 640
#define HEIGHT 480

const ppm_color_t colors[] = {
    PPM_BLACK,
    PPM_BLUE,
    PPM_GREEN,
    PPM_CYAN,

    PPM_RED,
    PPM_MAGENTA,
    PPM_BROWN,
    PPM_LIGHT_GREY,

    PPM_DARK_GREY,
    PPM_BRIGHT_BLUE,
    PPM_BRIGHT_GREEN,
    PPM_BRIGHT_CYAN,

    PPM_BRIGHT_RED,
    PPM_BRIGHT_MAGENTA,
    PPM_YELLOW,
    PPM_WHITE
};
const int colornum = sizeof(colors) / sizeof(colors[0]);

int main(int argc, char *argv[])
{
    char *output_filename = "output.ppm";

    int max_iterations = 512;
    int max_size = 4;

    int c;
    while ((c = getopt(argc, argv, "o:i:s:")) != -1) {
        char *endptr;

        switch (c) {
        case 'o':
            output_filename = optarg;
            break;
        case 'i':
            max_iterations = strtol(optarg, &endptr, 10);
            break;
        case 's':
            max_size = strtol(optarg, &endptr, 10);
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    ppm_t *ppm = ppm_create(WIDTH, HEIGHT);

    /*
     * dimensions of the complex plane
     */
    float x_min = -2.0;
    float x_max = 2.0;
    float y_min = -2.0;
    float y_max = 2.0;

    /*
     * For each row and each column set real and imag parts of the complex
     * number to be used in iteration
     */
    float delta_x = (x_max - x_min) / WIDTH;
    float delta_y = (y_max - y_min) / HEIGHT;

    float Q[HEIGHT] = { y_max };
    float P[WIDTH] = { x_min };
    for (int row = 1; row < HEIGHT; row++)
        Q[row] = Q[row - 1] - delta_y;
    for (int col = 1; col < WIDTH; col++ )
        P[col] = P[col - 1] + delta_x;

    /*
     * For every pixel calculate resulting value until the number becomes too
     * big, or we run out of iterations
     */
    for (int col = 0; col < WIDTH; col++ ) {
        for (int row = 0; row < HEIGHT; row++ ) {
            float x_square = 0.0;
            float y_square = 0.0;
            float x = 0.0;
            float y = 0.0;

            int color = 1;
            while (color < max_iterations && x_square + y_square < max_size) {
                x_square = x * x;
                y_square = y * y;
                y = 2 * x * y + Q[row];
                x = x_square - y_square + P[col];
                color++;
            }
            ppm_dot_safe(ppm, col, row, colors[color % colornum]);
        }
    }

    FILE *f = fopen(output_filename, "w");
    ppm_write(ppm, f);
    fclose(f);

    ppm_destroy(ppm);

    return EXIT_SUCCESS;
}
