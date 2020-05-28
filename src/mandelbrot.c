#include <complex.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "mandelbrot.h"

#define N 1000.0
#define N_THREADS 4

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

Color get_color_sqrt(double n, double complex z);
Color get_color(double n, double complex z);
Color get_color_from_pallete(double n, double complex z);
Color get_color_continuous(double n, double complex z);
coloring funcs[] = { &get_color_sqrt, &get_color, &get_color_from_pallete, &get_color_continuous };

typedef struct thread_args_struct {
    Resolution resolution;
    WorkPart x_y;
    Region region;
    unsigned char *pix;
    ColorAction action;
} ThreadArgs;


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
    n = sqrt(n / N);
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
    return hsv_to_rgb(360 * n / N, 1, 1);
}

Color get_color_from_pallete(double n, double complex z) {
    (void) z; /* unused */
    return colors[(int)n % n_colors];
}

void set_and_increment(unsigned char **pix, Color color) {
    *(*pix)++ = color.r;
    *(*pix)++ = color.g;
    *(*pix)++ = color.b;
    
}

void set_black_and_increment(unsigned char **pix) {
    set_and_increment(pix, BLACK);
}

void calculate_pixel(double complex c, unsigned char **pix, ColorAction color_action) {
    double complex z = 0.0;
    int number_of_iterations = 0;

    for(int n = 0; n < N; n++) {
        if(cabs(z) > 2) {
            Color c = funcs[color_action](n, z);
            set_and_increment(pix, c);
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

void *do_work(void *input) {
    ThreadArgs args = *(ThreadArgs*)input;

    for(unsigned int y = args.x_y.y_start; y < args.x_y.y_end; y++) {
        for(unsigned int x = args.x_y.x_start; x < args.x_y.x_end; x++) {
            double complex c = calculate_complex(x, y, args.resolution, args.region);
            calculate_pixel(c, &args.pix, args.action);
        }
    }
    return NULL;
}

void create_mandelbrot(Resolution resolution, Region region, unsigned char *pix, ColorAction action) {
    ThreadArgs *args1 = (ThreadArgs *)malloc(sizeof(ThreadArgs));
    ThreadArgs *args2 = (ThreadArgs *)malloc(sizeof(ThreadArgs));
    ThreadArgs *args3 = (ThreadArgs *)malloc(sizeof(ThreadArgs));
    ThreadArgs *args4 = (ThreadArgs *)malloc(sizeof(ThreadArgs));
    
    int steps = resolution.y / 4;

    WorkPart x_y = { .x_start = 0, .x_end = resolution.x, .y_start = 0, .y_end = steps };

    args1->resolution = resolution;
    args1->region = region;
    args1->pix = pix;
    args1->action = action;
    args1->x_y = x_y;
    

    x_y.y_start = x_y.y_end + 1;
    x_y.y_end += steps;
    // multiply by 3 for r,g,b values
    pix += steps * resolution.x * sizeof(unsigned char) * 3;

    args2->resolution = resolution;
    args2->region = region;
    args2->pix = pix;
    args2->action = action;
    args2->x_y = x_y;
    

    x_y.y_start = x_y.y_end + 1;
    x_y.y_end += steps;
    pix += (steps -1) * resolution.x * sizeof(unsigned char) * 3;


    args3->resolution = resolution;
    args3->region = region;
    args3->pix = pix;
    args3->action = action;
    args3->x_y = x_y;

    x_y.y_start = x_y.y_end + 1;
    x_y.y_end += steps;
    pix += (steps - 1) * resolution.x * sizeof(unsigned char) * 3;

    args4->resolution = resolution;
    args4->region = region;
    args4->pix = pix;
    args4->action = action;
    args4->x_y = x_y;


    pthread_t tid1;
    pthread_create(&tid1, NULL, do_work, (void *)args1);
    pthread_t tid2;
    pthread_create(&tid2, NULL, do_work, (void *)args2);
    pthread_t tid3;
    pthread_create(&tid3, NULL, do_work, (void *)args3);
    pthread_t tid4;
    pthread_create(&tid4, NULL, do_work, (void *)args4);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    free(args1);
    free(args2);
    free(args3);
    free(args4);
}
