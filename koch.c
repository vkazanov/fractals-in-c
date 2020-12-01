#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "draw.h"

#define GENERATOR_SIZE 5

void generate(float X1, float Y1, float X2, float Y2, int level);


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

    pbm = pbm_create(640, 480);

    /* Initiator points */
    const int initiator_size = 3;
    const int initiator_x1[] = {-150, 0, 150};
    const int initiator_y1[] = {-75, 185, -75};
    const int initiator_x2[] = {0, 150, -150};
    const int initiator_y2[] = {185, -75, -75};

    /* Kick of calculations with predefined initiator points */
    for (int i = 0; i < initiator_size; ++i) {
        generate(
            initiator_x1[i] + 320, initiator_y1[i] + 240,
            initiator_x2[i] + 320, initiator_y2[i] + 240,
            level
        );
    }

    FILE *f = fopen(output_filename, "w");
    pbm_write(pbm, f);
    fclose(f);

    pbm_destroy(pbm);

    return EXIT_SUCCESS;
}

void generate(float X1, float Y1, float X2, float Y2, int level)
{
    float Xpoints[GENERATOR_SIZE], Ypoints[GENERATOR_SIZE];
    level--;

    /* beginning/end of the line to be replaced */
    Xpoints[0] = X1;
    Ypoints[0] = Y1;

    Xpoints[4] = X2;
    Ypoints[4] = Y2;

    /* put and orient the turtle */
    turtle_t turtle = {
        .r = sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1)) / 3.0,
        .x = X1,
        .y = Y1,
    };
    turtle_point(&turtle, X1, Y1, X2, Y2);

    /* find intermediate steps */
    turtle_step(&turtle);
    Xpoints[1] = turtle.x;
    Ypoints[1] = turtle.y;

    turtle_turn(&turtle, 60);
    turtle_step(&turtle);
    Xpoints[2] = turtle.x;
    Ypoints[2] = turtle.y;
    turtle_turn(&turtle, -120);

    turtle_step(&turtle);
    Xpoints[3] = turtle.x;
    Ypoints[3] = turtle.y;

    /* either go deeper, or draw */
    if (level > 0) {
        for (int i = 0; i < GENERATOR_SIZE - 1; i++) {
            X1 = Xpoints[i];
            Y1 = Ypoints[i];
            X2 = Xpoints[i + 1];
            Y2 = Ypoints[i + 1];

            generate(X1, Y1, X2, Y2, level);
        }
    } else {
        for (int i = 0; i < GENERATOR_SIZE - 1; i++){
            pbm_line_safe(
                pbm,
                Xpoints[i], Ypoints[i],
                Xpoints[i + 1], Ypoints[i + 1]
            );
        }
    }
}
