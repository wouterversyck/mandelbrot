#ifndef _MANDELBROT_H
#define _MANDELBROT_H	1

typedef struct resolution_struct {
    unsigned int x;
    unsigned int y;
} Resolution;

typedef struct point_struct {
    unsigned int x;
    unsigned int y;
} Point;

typedef struct work_part_struct {
    unsigned int y_start;
    unsigned int y_end;
    unsigned int x_start;
    unsigned int x_end;
    unsigned char *pix;
} WorkPart;

typedef struct region_struct {
    double y_start;
    double y_end;
    double x_start;
    double x_end;
} Region;

typedef enum {
    SQRT,
    COUNT,
    PALETTE,
    CONTINUOUS
} ColorAction;

typedef struct configuration {
    Resolution resolution;
    WorkPart x_y;
    Region region;
    unsigned char *pix;
    ColorAction action;
    unsigned int nthreads;
} Configuration;

void create_mandelbrot(Configuration configuration);

#endif /* mandelbrot.h */