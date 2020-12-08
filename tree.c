#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

#define WIDTH 640
#define HEIGHT 480

pbm_t *pbm = NULL;

float left_alpha = 2.7;
float left_angle = 30;
float right_alpha = 2.7;
float right_angle = 30;

float left_height_factor = 0;
float right_height_factor = 0;

void generate(point_t beg, float theta, float height, int level);

int main(int argc, char *argv[])
{
    char *output_filename = "output.pbm";

    int level = 1;
    float height = 100;

    int c;
    while ((c = getopt(argc, argv, "o:l:h:1:2:3:4:")) != -1) {
        char *endptr;
        switch (c) {
        case 'o':
            output_filename = optarg;
            break;
        case 'l':
            level = strtol(optarg, &endptr, 10);
            break;
        case 'h':
            height = strtol(optarg, &endptr, 10);
            break;
        case '1':
            left_alpha = strtof(optarg, &endptr);
            break;
        case '2':
            left_angle = strtof(optarg, &endptr);
            break;
        case '3':
            right_alpha = strtof(optarg, &endptr);
            break;
        case '4':
            right_angle = strtof(optarg, &endptr);
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    left_height_factor = pow(2, -2 / (3 * left_alpha));
    right_height_factor = pow(2, -2 / (3 * right_alpha));

    pbm = pbm_create(WIDTH, HEIGHT);

    point_t beg = {.x = WIDTH / 2 + 0, .y = HEIGHT / 2 + 235};
    point_t end = {.x = WIDTH / 2 + 0, .y = beg.y - height};
    turtle_t turtle = {
        .x = beg.x,
        .y = beg.y,
        .r = 0
    };

    pbm_line_safe(pbm, beg.x, beg.y, end.x, end.y);

    turtle_point(&turtle, beg, end);
    turtle_turn(&turtle, left_angle);
    generate(end, turtle.theta, left_height_factor * height, level);

    turtle_point(&turtle, beg, end);
    turtle_turn(&turtle, -right_angle);
    generate(end, turtle.theta, right_height_factor * height, level);

    FILE *f = fopen(output_filename, "w");
    pbm_write(pbm, f);
    fclose(f);

    pbm_destroy(pbm);

    return EXIT_SUCCESS;
}

void generate(point_t beg, float theta, float height, int level)
{
    level--;

    turtle_t turtle = {
        .x = beg.x,
        .y = beg.y,
        .r = height,
        .theta = theta
    };
    turtle_step(&turtle);
    point_t end = {.x = turtle.x, .y = turtle.y};
    pbm_line_safe(pbm, beg.x, beg.y, end.x, end.y);;

    if (level > 0) {
        turtle_point(&turtle, beg, end);
        turtle_turn(&turtle, left_angle);
        generate(end, turtle.theta, left_height_factor * height, level);

        turtle_point(&turtle, beg, end);
        turtle_turn(&turtle, -right_angle);
        generate(end, turtle.theta, right_height_factor * height, level);
    }
}
