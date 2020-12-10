#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define EGA_WIDTH 640
#define EGA_HEIGHT 350

/* Utils */

typedef struct {
    float x;
    float y;
} point_t;

/* Portable BitMap (black & white, 0/1 per pixel)  */

typedef struct pbm_t {
    uint16_t width;
    uint16_t height;
    uint8_t *buf;
} pbm_t;

pbm_t *pbm_create(uint16_t width, uint16_t height);

void pbm_destroy(pbm_t *pbm);

void pbm_write(pbm_t *pbm, FILE *f);

void pbm_set(pbm_t *pbm, uint32_t x, uint32_t y, uint8_t val);

void pbm_dot(pbm_t *pbm, uint32_t x, uint32_t y);

void pbm_dot_safe(pbm_t *pbm, int x, int y);

int pbm_get(pbm_t *pbm, int x, int y);

void pbm_line(pbm_t *pbm,
              uint32_t x_begin, uint32_t y_begin,
              uint32_t x_end, uint32_t y_end);

void pbm_line_safe(pbm_t *pbm,
                   int x_begin, int y_begin,
                   int x_end, int y_end);

/* Portable PixMap (RGB, 3 values per pixel, 0-255 each) */

typedef struct ppm_color_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} ppm_color_t;

#define PPM_COLOR(red, green, blue) (ppm_color_t) {(red), (green), (blue)}

#define PPM_BLACK PPM_COLOR(0x00, 0x00, 0x00)
#define PPM_BLUE PPM_COLOR(0x00, 0x00, 0xAA)
#define PPM_GREEN PPM_COLOR(0x00, 0xAA, 0x00)
#define PPM_CYAN PPM_COLOR(0x00, 0xAA, 0xAA)

#define PPM_RED PPM_COLOR(0xAA, 0x00, 0x00)
#define PPM_MAGENTA PPM_COLOR(0xAA, 0x00, 0xAA)
#define PPM_BROWN PPM_COLOR(0xAA, 0x55, 0x00)
#define PPM_LIGHT_GREY PPM_COLOR(0xAA, 0xAA, 0xAA)

#define PPM_DARK_GREY PPM_COLOR(0x55, 0x55, 0x55)
#define PPM_BRIGHT_BLUE PPM_COLOR(0x55, 0x55, 0xFF)
#define PPM_BRIGHT_GREEN PPM_COLOR(0x55, 0xFF, 0x55)
#define PPM_BRIGHT_CYAN PPM_COLOR(0x55, 0xFF, 0xFF)

#define PPM_BRIGHT_RED PPM_COLOR(0xFF, 0x55, 0x55)
#define PPM_BRIGHT_MAGENTA PPM_COLOR(0xFF, 0x55, 0xFF)
#define PPM_YELLOW PPM_COLOR(0xFF, 0xFF, 0x55)
#define PPM_WHITE PPM_COLOR(0xFF, 0xFF, 0xFF)

typedef struct ppm_t {
    uint16_t width;
    uint16_t height;
    ppm_color_t *buf;
} ppm_t;

ppm_t *ppm_create(uint32_t width, uint32_t height);

void ppm_destroy(ppm_t *ppm);

void ppm_write(ppm_t *ppm, FILE *f);

void ppm_dot_safe(ppm_t *ppm, int x, int y, ppm_color_t color);

void ppm_dot(ppm_t *ppm, uint32_t x, uint32_t y, ppm_color_t color);

/* Turtle graphics */

typedef struct turtle_t {
    float x, y, r, theta;
} turtle_t;

void turtle_point(turtle_t *turtle, point_t beg, point_t end);

void turtle_turn(turtle_t *turtle, float angle);

point_t turtle_pos(turtle_t *turtle);

point_t turtle_step(turtle_t *turtle);


#endif /* UTIL_H */
