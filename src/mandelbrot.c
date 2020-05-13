#include "includes.h"


#define N 1000

const Color YELLOW_3 = { .r = 255, .g = 170, .b = 0 };
const Color YELLOW_2 = { .r = 248, .g = 201, .b = 95 };
const Color YELLOW_1 = { .r = 241, .g = 233, .b = 191 };
const Color BROWN_4 = { .r = 66, .g = 30, .b = 15 };
const Color BROWN_3 = { .r = 106, .g = 52, .b = 3 };
const Color BROWN_2 = { .r = 153, .g = 87, .b = 0 };
const Color BROWN_1 = { .r = 204, .g = 128, .b = 0 };
const Color DARK_VIOLET = { .r = 25, .g = 7, .b = 26 };
const Color BLUE_7 = { .r = 9, .g = 1, .b = 47 };
const Color BLUE_6 = { .r = 4, .g = 4, .b = 73 };
const Color BLUE_5 = { .r = 0, .g = 7, .b = 100 };
const Color BLUE_4 = { .r = 12, .g = 44, .b = 138 };
const Color BLUE_3 = { .r = 24, .g = 82, .b = 177 };
const Color BLUE_2 = { .r = 57, .g = 125, .b = 209 };
const Color BLUE_1 = { .r = 134, .g = 181, .b = 229 };
const Color BLUE_0 = { .r = 211, .g = 236, .b = 248 };
const Color BLACK = { .r = 0, .g = 0, .b = 0 };

Color colors[] = {YELLOW_3, YELLOW_2, YELLOW_1, BROWN_4, BROWN_3, BROWN_2, BROWN_1, DARK_VIOLET, BLUE_7, BLUE_6, BLUE_5, BLUE_4, BLUE_3, BLUE_2, BLUE_1, BLUE_0, BLACK};

#define n_colors sizeof(colors) / sizeof(Color)


void set_and_increment(unsigned char **pix, Color color) {
    *(*pix)++ = color.r;
    *(*pix)++ = color.g;
    *(*pix)++ = color.b;
}

void set_black_and_increment(unsigned char **pix) {
    set_and_increment(pix, BLACK);
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

double complex calculate_complex(double x, double y, Resolution resolution, Region region) {
    const double xi = (region.x_start + (x / resolution.x) * (region.x_end - region.x_start));
    const double yi = (region.y_start + (y / resolution.y) * (region.y_end - region.y_start));
    return xi + yi * I;
}

void create_mandelbrot(Resolution resolution, Region region, unsigned char **pix) {
    for(int y = 0; y < resolution.y; y++) {
        for(int x = 0; x < resolution.x; x++) {
            double complex c = calculate_complex(x, y, resolution, region);
            calculate_pixel(c, pix);
        }
    }
}

