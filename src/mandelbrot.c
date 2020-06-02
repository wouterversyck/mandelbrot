#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "mandelbrot.h"
#include "color_utils.h"
#include "global.h"

double n_iterations = 1000;
coloring funcs[] = { &get_color_sqrt, &get_color, &get_color_from_pallete, &get_color_continuous };
Configuration configuration;

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

    for(int n = 0; n < n_iterations; n++) {
        if(cabs(z) > 2) {
            Color c = funcs[color_action](n, z);
            set_and_increment(pix, c);
            break;
        }
        
        number_of_iterations++;
        z = z * z + c;
    }
    if(number_of_iterations == n_iterations) {
        set_black_and_increment(pix);
    }
}

double complex calculate_complex(double x, double y, Resolution resolution, Region region) {
    const double xi = (region.x_start + (x / resolution.x) * (region.x_end - region.x_start));
    const double yi = (region.y_start + (y / resolution.y) * (region.y_end - region.y_start));
    return xi + yi * I;
}

void *do_work(void *input) {
    WorkPart args = *(WorkPart*)input;

    for(unsigned int y = args.y_start; y < args.y_end; y++) {
        for(unsigned int x = args.x_start; x < args.x_end; x++) {
            double complex c = calculate_complex(x, y, configuration.resolution, configuration.region);
            calculate_pixel(c, &args.pix, configuration.action);
        }
    }
    return NULL;
}

void create_mandelbrot(Configuration conf) {
    configuration = conf;

    unsigned int steps = conf.resolution.y / conf.n_threads;
    pthread_t thread_ids[conf.n_threads];
    WorkPart *args[conf.n_threads];

    unsigned int x_start = 0, x_end = conf.resolution.x, y_start = 0, y_end = steps;
    unsigned char* pix = conf.pix;

    for(unsigned int i = 0; i < conf.n_threads; i++) {
        WorkPart *t_arg = (WorkPart *)malloc(sizeof(WorkPart));
        t_arg->x_start = x_start;
        t_arg->x_end = x_end;
        t_arg->y_start = y_start;
        t_arg->y_end = y_end;
        t_arg->pix = pix;

        args[i] = t_arg;

        y_start = y_end;
        y_end += steps;
        // multiply by 3 for r,g,b values
        pix += steps * conf.resolution.x * sizeof(unsigned char) * 3;

        pthread_create(&thread_ids[i], NULL, do_work, (void *) t_arg);
    }

    for(unsigned int i = 0; i < conf.n_threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }
    for(unsigned int i = 0; i < conf.n_threads; i++) {
        free(args[i]);
    }
}
