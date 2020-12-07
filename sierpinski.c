#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

#define WIDTH 640
#define HEIGHT 480

pbm_t *pbm = NULL;

void generate(point_t beg, point_t end, int level, int sign);

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

    int init_size = 1;
    point_t beg[] = {
        {WIDTH / 2 - 130, HEIGHT / 2 - 0},
    };
    point_t end[] = {
        {WIDTH / 2 + 130, HEIGHT / 2 + 0},
    };

    for (int i = 0; i < init_size; ++i) {
        generate(beg[i], end[i], level, 1);
    }

    FILE *f = fopen(output_filename, "w");
    pbm_write(pbm, f);
    fclose(f);

    pbm_destroy(pbm);

    return EXIT_SUCCESS;
}

void generate(point_t beg, point_t end, int level, int sign)
{
#define GENERATOR_SIZE 4

    point_t points[GENERATOR_SIZE];
    level--;

    turtle_t turtle = {
        .r = sqrt((end.x - beg.x) * (end.x - beg.x) +
                  (end.y - beg.y) * (end.y - beg.y)) / 2.0,
        .x = beg.x,
        .y = beg.y,
    };

    points[0] = beg;
    points[3] = end;
    turtle_point(&turtle, beg, end);

    turtle_turn(&turtle, 60 * sign);
    points[1] = turtle_step(&turtle);

    turtle_turn(&turtle, -60 * sign);
    points[2] = turtle_step(&turtle);

    sign *= -1;

    if (level > 0) {
        int int_sign = sign;
        for (int i = 0; i < GENERATOR_SIZE - 1; i++) {
            generate(points[i], points[i + 1], level, int_sign);
            int_sign *= -1;
        }
    } else {
        for (int i = 0; i < GENERATOR_SIZE - 1; i++)
            pbm_line_safe(
                pbm,
                points[i].x, points[i].y,
                points[i + 1].x, points[i + 1].y
            );
    }

#undef GENERATOR_SIZE
}
