#include <stdio.h>
#include <imago2.h>
#include <complex.h>
#include <time.h>

#include "mandelbrot.h"


const Resolution TT_K = { .x = 30720, .y = 17280 };
const Resolution ST_K = { .x = 15360, .y = 8640 };
const Resolution EIGHT_K = { .x = 7680, .y = 4320 };
const Resolution FOUR_K = { .x = 3840, .y = 2160 };
const Resolution FULL_HD = { .x = 1920, .y = 1080 };

const Region REGION_FULL = { .y_start = -1.0, .y_end = 1.0, .x_start = -2.5, .x_end = 0.9 };
const Region REGION_COOL = { .y_start = 0.2305, .y_end = 0.2495, .x_start = -0.738, .x_end = -0.706 };


void print_time() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void print_start() {
    printf("Starting at: ");
    print_time();
}

void print_end() {
    printf("Ending at: ");
    print_time();
}

int main() {
    const char *outfile = "out.jpg";
    
    Resolution resolution = FULL_HD;
    ColorAction action = CONTINUOUS;
    Region region = REGION_COOL;

    struct img_pixmap img;

    img_init(&img);

    if(img_set_pixels(&img, resolution.x, resolution.y, IMG_FMT_RGB24, 0) == -1) {
        perror("The what!!!");
        return 1;
    }

    print_start();

    unsigned char *pix = img.pixels;
    create_mandelbrot(resolution, region, &pix, action);

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
