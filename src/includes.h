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

typedef struct region_struct {
    double y_start;
    double y_end;
    double x_start;
    double x_end;
} Region;

#endif /* includes.h */