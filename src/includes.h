#include <complex.h>
struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

const struct Color RED = { .r = 255, .g = 0, .b = 0 };
const struct Color GREEN = { .r = 0, .g = 255, .b = 0 };
const struct Color BLUE = { .r = 0, .g = 0, .b = 255 };
const struct Color BLACK = { .r = 0, .g = 0, .b = 0 };
const struct Color GREY = { .r = 100, .g = 100, .b = 100 };
const struct Color WHITE = { .r = 255, .g = 255, .b = 255 };

struct Color colors[] = {RED, GREEN, BLUE, BLACK, WHITE, GREY};
#define n_colors sizeof(colors) / sizeof(struct Color)

void set_black_and_increment(unsigned char **pix);
void set_white_and_increment(unsigned char **pix);
void set_and_increment(unsigned char **pix, struct Color color);
double complex calculate_complex(double i, double j, double xsz, double ysz);
void calculate_pixel(double complex c, unsigned char **pix);
