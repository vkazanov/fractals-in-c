#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "draw.h"

#define WIDTH 640
#define HEIGHT 480

pbm_t *pbm = NULL;

void generate(float r1, float r2, int level);

point_t point1, point2;

int main(int argc, char *argv[])
{

    char *output_filename = "output.pbm";

    int level = 1;

    int c;
    while ((c = getopt(argc, argv, "o:l:")) != -1) {
        char *endptr;
        switch (c) {
        case 'o':
            output_filename = optarg;
            break;
        case 'l':
            level = strtol(optarg, &endptr, 10);
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    pbm = pbm_create(WIDTH, HEIGHT);

    float r = 400 / pow(2, level);
    point1 = (point_t){ .x = WIDTH / 2 - 200, .y = HEIGHT / 2 - 200 };
    point2 = (point_t){ .x = WIDTH / 2 - 200, .y = HEIGHT / 2 - 200 };

    generate(r, 0, level - 1);

    FILE *f = fopen(output_filename, "w");
    pbm_write(pbm, f);
    fclose(f);

    pbm_destroy(pbm);

    return EXIT_SUCCESS;
}

void generate(float r1, float r2, int level)
{
    if (level > 0)
        generate(r2, r1, level - 1);

    point2.x += r1;
    point2.y += r2;
    pbm_line_safe(
        pbm,
        point1.x, point1.y,
        point2.x, point2.y
    );
    point1 = point2;
    if (level > 0)
        generate(r1, r2, level - 1);

    point2.x += r2;
    point2.y += r1;
    pbm_line_safe(
        pbm,
        point1.x, point1.y,
        point2.x, point2.y
    );
    point1 = point2;
    if (level > 0)
        generate(r1, r2, level - 1);

    point2.x -= r1;
    point2.y -= r2;
    pbm_line_safe(
        pbm,
        point1.x, point1.y,
        point2.x, point2.y
    );
    point1 = point2;
    if (level > 0)
        generate(-r2, -r1, level - 1);
}
