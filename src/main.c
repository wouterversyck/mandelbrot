#include <stdio.h>
#include <imago2.h>
#include <complex.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <stdbool.h>

#include "ptime.h"
#include "mandelbrot.h"


const Resolution TT_K = { .x = 30720, .y = 17280 };
const Resolution ST_K = { .x = 15360, .y = 8640 };
const Resolution EIGHT_K = { .x = 7680, .y = 4320 };
const Resolution FOUR_K = { .x = 3840, .y = 2160 };
const Resolution FULL_HD = { .x = 1920, .y = 1080 };

const Region REGION_FULL = { .y_start = -1.0, .y_end = 1.0, .x_start = -2.5, .x_end = 0.9 };
const Region REGION_COOL = { .y_start = 0.2305, .y_end = 0.2495, .x_start = -0.738, .x_end = -0.706 };

char outfile[50] = "out.jpg";
unsigned int nthreads = 0;

void usage() {
    printf("Required arguments:\n");
    printf("-t = number of arguments (defaults to ncpus)\n");
    printf("-o = output file (extension jpg or png will determine output format)\n");
}

bool parse_args(int argc, char *argv[]) {
    while ((argc > 1) && (argv[1][0] == '-')){
		switch (argv[1][1]) {
			case 't':
                nthreads = atoi(&argv[1][2]);
				break;

			case 'o':
                strcpy(outfile, &argv[1][2]);
				break;

            case 'h':
			default:
				usage();
                return false;
		}

		++argv;
		--argc;
	}

    if(nthreads == 0) {
        nthreads = get_nprocs();
    }
    printf("Number of threads: %d\n", nthreads);
    printf("Saving output to %s\n", outfile);

    return true;
}

int main(int argc, char *argv[]) {
    if(!parse_args(argc, argv)) {
        return 0;
    }
    
    Resolution resolution = FULL_HD;
    ColorAction action = SQRT;
    Region region = REGION_COOL;

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
        .nthreads = nthreads
    };

    create_mandelbrot(config);

    print_end();

    printf("Saving image\n");

    if(img_save(&img, outfile) == -1) {
        fprintf(stderr, "Failed to save file %s\n", outfile);
        return 1;
    }

    printf("Completed\n");

    img_destroy(&img);
    return 0;
}
