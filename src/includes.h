#ifndef _INCLUDES_H
#define _INCLUDES_H	1

#include <complex.h>


struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct Resolution {
    unsigned int x;
    unsigned int y;
};

void set_black_and_increment(unsigned char **pix);
void set_white_and_increment(unsigned char **pix);
void set_and_increment(unsigned char **pix, struct Color color);
double complex calculate_complex(double i, double j, double xsz, double ysz);
void calculate_pixel(double complex c, unsigned char **pix);

#endif /* includes.h */