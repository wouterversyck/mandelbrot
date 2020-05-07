#include <stdio.h>
#include <imago2.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>

#include "includes.h"

#define N 1000

int main() {
    const char *outfile = "out.jpg";
	int xsz = 2048, ysz = 2048;
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
            double complex c = calculate_complex(y, x, xsz + 1000, ysz + 1000);
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
    set_and_increment(pix, 0, 0, 0);
}

void set_white_and_increment(unsigned char **pix) {
    set_and_increment(pix, 255, 255, 255);
}

void set_and_increment(unsigned char **pix, unsigned char r, unsigned char g, unsigned char b) {
    *(*pix)++ = r;
    *(*pix)++ = g;
    *(*pix)++ = b;
}

void calculate_pixel(double complex c, unsigned char **pix) {
    
    double complex z = 0.0;

    for(int n = 0; n < N; n++) {
        if(cabs(z) > 50) {
            set_and_increment(pix, 125, 70, 25);
            goto label;
        }
        if(cabs(z) > 10) {
            set_and_increment(pix, 58, 70, 125);
            goto label;
        }
        if(cabs(z) > 4) {
            set_white_and_increment(pix);
            goto label;
        }
        
        z = z*z+c;
    }
    set_black_and_increment(pix);
    label:{};
}

double complex calculate_complex(int i, int j, int xsz, int ysz) {
    return (double)i / xsz + (double)j / ysz * I;
}
