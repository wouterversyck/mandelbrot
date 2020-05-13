#ifndef _INCLUDES_H
#define _INCLUDES_H	1

#include <complex.h>


typedef struct color_struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

typedef struct resolution_struct {
    unsigned int x;
    unsigned int y;
} Resolution;

void set_black_and_increment(unsigned char **pix);
void set_white_and_increment(unsigned char **pix);
void set_and_increment(unsigned char **pix, Color color);
double complex calculate_complex(double i, double j, double xsz, double ysz);
void calculate_pixel(double complex c, unsigned char **pix);
void print_time();
void print_start();
void print_end();

#endif /* includes.h */