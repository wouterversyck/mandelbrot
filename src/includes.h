#include <complex.h>

void set_black_and_increment(unsigned char **pix);
void set_white_and_increment(unsigned char **pix);
void set_and_increment(unsigned char **pix, unsigned char r, unsigned char g, unsigned char b);
double complex calculate_complex(int i, int j, int xsz, int ysz);
void calculate_pixel(double complex c, unsigned char **pix);