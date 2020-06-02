#include <stdio.h>
#include <imago2.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <stdbool.h>
#include <getopt.h>

#include "ptime.h"
#include "mandelbrot.h"


static const Resolution TT_K = { .x = 30720, .y = 17280 };
static const Resolution ST_K = { .x = 15360, .y = 8640 };
static const Resolution EIGHT_K = { .x = 7680, .y = 4320 };
static const Resolution FOUR_K = { .x = 3840, .y = 2160 };
static const Resolution FULL_HD = { .x = 1920, .y = 1080 };

static const Resolution resolutions[] = { FULL_HD, FOUR_K, EIGHT_K, ST_K, TT_K };

static const Region REGION_FULL = { .y_start = -1.0, .y_end = 1.0, .x_start = -2.5, .x_end = 0.9 };
static const Region REGION_COOL = { .y_start = 0.2305, .y_end = 0.2495, .x_start = -0.738, .x_end = -0.706 };

static const Region regions[] = { REGION_FULL, REGION_COOL };

static char outfile[50] = "out.jpg";
static unsigned int n_threads = 0;
static Resolution resolution = FULL_HD;
static Region region = REGION_FULL;

double n_iterations = 1000;


static void usage() {
    printf("Arguments:\n");
    printf("-t = number of arguments (defaults to ncpus)\n");
    printf("-o = output file (extension jpg or png will determine output format)\n");
    printf("-n = number of iterations (defaults to 1000\n");
    printf("-r = resolution:\n");
    printf("     1- 1920x1080\n");
    printf("     2- 3840x2160\n");
    printf("     3- 7680x4320\n");
    printf("     4- 15360x1080\n");
    printf("     5- 30720x1080\n");
    printf("-i = region:\n");
    printf("     1- full mandelbrot view\n");
    printf("     2- Super cool region\n");
}

static Resolution get_resolution(unsigned int value) {
    if(value > (sizeof(resolutions) / sizeof(resolution)) - 1) {
        printf("Value for resolution %d is out of range, will use full hd\n\n", value);
        return FULL_HD;
    } 

    return resolutions[value];
}

static Region get_region(unsigned int value) {
    if(value > (sizeof(regions) / sizeof(region)) - 1) {
        printf("Value for reigon %d is out of range, will use super cool region\n\n", value);
        return REGION_FULL;
    } 

    return regions[value];
}

static bool parse_args(int argc, char **argv) {
    int c;
    while ((c = getopt(argc, argv, "o:t:n:r:i:h")) != -1) {
		switch (c) {
			case 't':
                n_threads = strtol(optarg, NULL, 10);
				break;
			case 'o':
                snprintf(outfile, sizeof(outfile), "%s", optarg);
				break;
            case 'n':
                n_iterations = strtol(optarg, NULL, 10);
                break;
            case 'r':
                resolution = get_resolution(strtol(optarg, NULL, 10) -1);
                break;
            case 'i':
                region = get_region(strtol(optarg, NULL, 10) -1);
                break;
            case 'h':
				usage();
                return false;
		}
	}

    if(n_threads == 0) {
        n_threads = get_nprocs();
    }
    
    printf("----------------------------------------------------\n");
    printf("Number of iterations:\t%f\n", n_iterations);
    printf("Number of threads:\t%d\n", n_threads);
    printf("Resolution of image:\t%dx%d\n", resolution.x, resolution.y);
    printf("Saving output to:\t%s\n", outfile);
    printf("----------------------------------------------------\n\n");

    return true;
}

int main(int argc, char **argv) {
    if(!parse_args(argc, argv)) {
        return 0;
    }
    
    ColorAction action = SQRT;

    struct img_pixmap img;

    img_init(&img);

    if(img_set_pixels(&img, resolution.x, resolution.y, IMG_FMT_RGB24, 0) == -1) {
        perror("Could not open image");
        return 1;
    }

    print_start();

    Configuration config = { 
        .pix = img.pixels,
        .resolution = resolution,
        .region = region,
        .action = action,
        .n_threads = n_threads
    };

    create_mandelbrot(config);

    print_end();

    printf("\nSaving image\n");

    if(img_save(&img, outfile) == -1) {
        fprintf(stderr, "Failed to save file %s\n", outfile);
        return 1;
    }

    printf("Completed\n");

    img_destroy(&img);
    return 0;
}
