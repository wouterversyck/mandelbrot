#include <stdio.h>
#include <imago2.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>

#include "includes.h"

#define N 1000
#define RE_START -2.0
#define RE_END -0.0
#define IM_START -1.5
#define IM_END 1.5

int main() {
    const char *outfile = "out.jpg";
	int xsz = 1920, ysz = 1920;
	struct img_pixmap img;

    img_init(&img);

    if(img_set_pixels(&img, xsz, ysz, IMG_FMT_RGB24, 0) == -1) {
        perror("The what!!!");
        return 1;
    }

    unsigned char *pix;
    pix = img.pixels;
    for(int y = 0; y < ysz; y++) {
        for(int x = 0; x < xsz; x++) {
            double complex c = calculate_complex(x, y, xsz, ysz);
            calculate_pixel(c, &pix);
        }
    }

	if(img_save(&img, outfile) == -1) {
		fprintf(stderr, "failed to save file %s\n", outfile);
		return 1;
	}

	img_destroy(&img);
	return 0;
}

void set_black_and_increment(unsigned char **pix) {
    set_and_increment(pix, BLACK);
}

void set_white_and_increment(unsigned char **pix) {
    set_and_increment(pix, WHITE);
}

void set_and_increment(unsigned char **pix, struct Color color) {
    *(*pix)++ = color.r;
    *(*pix)++ = color.g;
    *(*pix)++ = color.b;
}

void calculate_pixel(double complex c, unsigned char **pix) {
    double complex z = 0.0;
    int number_of_iterations = 0;

    for(int n = 0; n < N; n++) {
        if(cabs(z) > 100) {
            set_and_increment(pix, colors[n % n_colors]);
            break;
        }
        
        number_of_iterations++;
        z = z * z + c;
    }
    if(number_of_iterations == N) {
        set_black_and_increment(pix);
    }
}

double complex calculate_complex(double x, double y, double xsz, double ysz) {
    const double xi = (RE_START + (x / xsz) * (RE_END - RE_START));
    const double yi = (IM_START + (y / ysz) * (IM_END - IM_START));
    return xi + yi * I;
}
