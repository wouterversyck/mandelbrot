#include <complex.h>
#include <math.h>
#include "color_utils.h"

extern double n_iterations;

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

Color colors[] = { YELLOW_3, YELLOW_2, YELLOW_1, BROWN_4, BROWN_3, BROWN_2, BROWN_1, DARK_VIOLET, BLUE_7, BLUE_6, BLUE_5, BLUE_4, BLUE_3, BLUE_2, BLUE_1, BLUE_0 };

#define n_colors sizeof(colors) / sizeof(Color)

Color hsv_to_rgb(double H, double S, double V) {
    double C = S * V;
    double X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
    double m = V - C;
    double Rs, Gs, Bs;

    if(H >= 0 && H < 60) {
        Rs = C;
        Gs = X;
        Bs = 0;	
    }
    else if(H >= 60 && H < 120) {	
        Rs = X;
        Gs = C;
        Bs = 0;	
    }
    else if(H >= 120 && H < 180) {
        Rs = 0;
        Gs = C;
        Bs = X;	
    }
    else if(H >= 180 && H < 240) {
        Rs = 0;
        Gs = X;
        Bs = C;	
    }
    else if(H >= 240 && H < 300) {
        Rs = X;
        Gs = 0;
        Bs = C;	
    }
    else {
        Rs = C;
        Gs = 0;
        Bs = X;	
    }

    Color c;
    c.r = (Rs + m) * 255;
    c.g = (Gs + m) * 255;
    c.b = (Bs + m) * 255;

    return c;
}


/** 
 * Sqrt to make lower values grow faster
 * Log is used to smooth out the edges
 **/
Color get_color_sqrt(double n, double complex z) {
    (void) z; /* unused */
    n = n + 1 - log(log2(cabs(z)));
    n = sqrt(n / n_iterations);
    return hsv_to_rgb(360 * n, 1, 1);
}

Color get_color_continuous(double n, double complex z) {
    double continuous_index = n + 1 - log(log2(cabs(z)));

    Color c;
    c.r = (sin(0.016 * continuous_index + 4) * 127.5 + 127.5);
    c.g = (sin(0.013 * continuous_index + 2) * 127.5 + 127.5);
    c.b = (sin(0.01 * continuous_index + 1) * 127.5 + 127.5);

    return c;
}

Color get_color(double n, double complex z) {
    (void) z; /* unused */
    return hsv_to_rgb(360 * n / n_iterations, 1, 1);
}

Color get_color_from_pallete(double n, double complex z) {
    (void) z; /* unused */
    return colors[(int)n % n_colors];
}
