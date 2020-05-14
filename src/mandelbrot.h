#ifndef _MANDELBROT_H
#define _MANDELBROT_H	1


typedef struct color_struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

typedef struct resolution_struct {
    unsigned int x;
    unsigned int y;
} Resolution;

typedef struct region_struct {
    double y_start;
    double y_end;
    double x_start;
    double x_end;
} Region;

typedef enum {
    SQRT,
    Count,
    PALETTE
} ColorAction;

typedef Color (*coloring)(double n);

void create_mandelbrot(Resolution resolution, Region region, unsigned char **pix, ColorAction action);

#endif /* mandelbrot.h */