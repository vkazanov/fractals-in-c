#include <stdlib.h>
#include <unistd.h>
#include "draw.h"


int main(int argc, char *argv[])
{
#define DOT(turtle) pbm_dot_safe(pbm, (int)(turtle).x,(int)(turtle).y)

    char *output_filename = "output.pbm";

    int c;
    while ((c = getopt(argc, argv, "o:")) != -1) {
        switch (c) {
        case 'o':
            output_filename = optarg;
            break;
        default:
            return EXIT_FAILURE;
        }
    }


    pbm_t *pbm = pbm_create(EGA_WIDTH, EGA_HEIGHT);

    turtle_t turtle = { .x = 320, .y = 175, .r = 50 };
    turtle_point(&turtle, 320, 175, 640, 175);

    DOT(turtle);

    turtle_step(&turtle);
    DOT(turtle);

    turtle_turn(&turtle, 90);
    DOT(turtle);

    turtle_step(&turtle);
    DOT(turtle);

    FILE *f = fopen(output_filename, "w");
    pbm_write(pbm, f);
    fclose(f);

    pbm_destroy(pbm);

    return EXIT_SUCCESS;

#undef DOT
}
