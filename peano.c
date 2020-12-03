#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "draw.h"

#define WIDTH 640
#define HEIGHT 480

void generate(point_t beg, point_t end, int level);

pbm_t *pbm = NULL;

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

    /* Initiator points */
    const int initiator_size = 1;
    const point_t initiator_beg[] = {
        { .x = WIDTH / 2 + 0, .y = HEIGHT / 2 - 100 },
    };
    const point_t initiator_end[] = {
        { .x = WIDTH / 2 + 0, .y = HEIGHT / 2  + 100 },
    };

    /* Kick of calculations with predefined initiator points */
    for (int i = 0; i < initiator_size; ++i)
        generate(initiator_beg[i], initiator_end[i], level);

    FILE *f = fopen(output_filename, "w");
    pbm_write(pbm, f);
    fclose(f);

    pbm_destroy(pbm);

    return EXIT_SUCCESS;
}

void generate(point_t beg, point_t end, int level)
{
#define GENERATOR_SIZE 10

    point_t points[GENERATOR_SIZE];
    level--;

    /* beginning/end of the line to be replaced */
    points[0] = beg;
    points[9] = end;

    /* find intermediate steps */
    turtle_t turtle = {
        .r = sqrt((end.x - beg.x) * (end.x - beg.x) +
                  (end.y - beg.y) * (end.y - beg.y)) / 3.0,
        .x = beg.x,
        .y = beg.y,
    };
    turtle_point(&turtle, beg, end);
    points[1] = turtle_step(&turtle);

    turtle_turn(&turtle, 90);
    points[2] = turtle_step(&turtle);

    turtle_turn(&turtle, -90);
    points[3] = turtle_step(&turtle);

    turtle_turn(&turtle, -90);
    points[4] = turtle_step(&turtle);

    turtle_turn(&turtle, -90);
    points[5] = turtle_step(&turtle);

    turtle_turn(&turtle, 90);
    points[6] = turtle_step(&turtle);

    turtle_turn(&turtle, 90);
    points[7] = turtle_step(&turtle);

    turtle_turn(&turtle, 90);
    points[8] = turtle_step(&turtle);

    /* either go deeper, or draw */
    if (level > 0) {
        for (int i = 0; i < GENERATOR_SIZE - 1; i++)
            generate(points[i], points[i + 1], level);
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
