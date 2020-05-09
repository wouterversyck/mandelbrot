#include <stdio.h>
#include <imago2.h>
#include <complex.h>
#include <time.h>

#include "includes.h"

#define N 1000
/* MAKE SURE THEY ARE AL IN A 1.7 RATIO */
/* Normal full mandelbrot region */
#define Y_START -1.0
#define Y_END 1.0
#define X_START -2.5
#define X_END 0.9

/* cool region */
/*
#define Y_START 0.2305
#define Y_END 0.2495
#define X_START -0.738
#define X_END -0.706
*/

const struct Color YELLOW_3 = { .r = 255, .g = 170, .b = 0 };
const struct Color YELLOW_2 = { .r = 248, .g = 201, .b = 95 };
const struct Color YELLOW_1 = { .r = 241, .g = 233, .b = 191 };
const struct Color BROWN_4 = { .r = 66, .g = 30, .b = 15 };
const struct Color BROWN_3 = { .r = 106, .g = 52, .b = 3 };
const struct Color BROWN_2 = { .r = 153, .g = 87, .b = 0 };
const struct Color BROWN_1 = { .r = 204, .g = 128, .b = 0 };
const struct Color DARK_VIOLET = { .r = 25, .g = 7, .b = 26 };
const struct Color BLUE_7 = { .r = 9, .g = 1, .b = 47 };
const struct Color BLUE_6 = { .r = 4, .g = 4, .b = 73 };
const struct Color BLUE_5 = { .r = 0, .g = 7, .b = 100 };
const struct Color BLUE_4 = { .r = 12, .g = 44, .b = 138 };
const struct Color BLUE_3 = { .r = 24, .g = 82, .b = 177 };
const struct Color BLUE_2 = { .r = 57, .g = 125, .b = 209 };
const struct Color BLUE_1 = { .r = 134, .g = 181, .b = 229 };
const struct Color BLUE_0 = { .r = 211, .g = 236, .b = 248 };
const struct Color BLACK = { .r = 0, .g = 0, .b = 0 };

const struct Resolution TT_K = { .x = 30720, .y = 17280 };
const struct Resolution ST_K = { .x = 15360, .y = 8640 };
const struct Resolution EIGHT_K = { .x = 7680, .y = 4320 };
const struct Resolution FOUR_K = { .x = 3840, .y = 2160 };
const struct Resolution FULL_HD = { .x = 1920, .y = 1080 };

struct Color colors[] = {YELLOW_3, YELLOW_2, YELLOW_1, BROWN_4, BROWN_3, BROWN_2, BROWN_1, DARK_VIOLET, BLUE_7, BLUE_6, BLUE_5, BLUE_4, BLUE_3, BLUE_2, BLUE_1, BLUE_0, BLACK};

#define n_colors sizeof(colors) / sizeof(struct Color)


int main() {
    const char *outfile = "out.jpg";
    struct Resolution resolution = FULL_HD;
    struct img_pixmap img;

    img_init(&img);

    if(img_set_pixels(&img, resolution.x, resolution.y, IMG_FMT_RGB24, 0) == -1) {
        perror("The what!!!");
        return 1;
    }

    print_start();

    unsigned char *pix;
    pix = img.pixels;
    for(int y = 0; y < resolution.y; y++) {
        for(int x = 0; x < resolution.x; x++) {
            double complex c = calculate_complex(x, y, resolution.x, resolution.y);
            calculate_pixel(c, &pix);
        }
    }

    print_end();

    printf("Saving image\n");

    if(img_save(&img, outfile) == -1) {
        fprintf(stderr, "failed to save file %s\n", outfile);
        return 1;
    }

    printf("Completed\n");

    img_destroy(&img);
    return 0;
}

void set_black_and_increment(unsigned char **pix) {
    set_and_increment(pix, BLACK);
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
        if(cabs(z) > 2) {
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
    const double xi = (X_START + (x / xsz) * (X_END - X_START));
    const double yi = (Y_START + (y / ysz) * (Y_END - Y_START));
    return xi + yi * I;
}

void print_start() {
    printf("Starting at: ");
    print_time();
}

void print_end() {
    printf("Ending at: ");
    print_time();
}

void print_time() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}
