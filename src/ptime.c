#include <stdio.h>
#include <time.h>
#include "ptime.h"

void print_time() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void print_start() {
    printf("Starting at:\t");
    print_time();
}

void print_end() {
    printf("Ending at:\t");
    print_time();
}