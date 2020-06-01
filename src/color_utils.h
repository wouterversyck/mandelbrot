#ifndef _COLOR_UTILS_H
#define _COLOR_UTILS_H	1

#include <complex.h>

typedef struct color_struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

extern const Color BLACK;

typedef Color (*coloring)(double n, double complex z);

Color get_color_sqrt(double n, double complex z);
Color get_color(double n, double complex z);
Color get_color_from_pallete(double n, double complex z);
Color get_color_continuous(double n, double complex z);

#endif /* mandelbrot.h */