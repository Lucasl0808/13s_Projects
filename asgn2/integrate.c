#include "functions.h"
#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "abcdefghijn:p:q:h"

int main(int argc, char **argv) {
    int opt = 0;
    long int starting_n = 2;
    char *pi;
    bool opt_a = false;
    bool opt_b = false;
    bool opt_c = false;
    bool opt_d = false;
    bool opt_e = false;
    bool opt_f = false;
    bool opt_g = false;
    bool opt_h = false;
    bool opt_i = false;
    bool opt_j = false;
    double low = 0.0;
    double high = 0.0;
    long int partitions = 100;
    double (*z)(double) = NULL;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            z = &a;
            opt_a = true;
            break;
        case 'b':
            z = &b;
            opt_b = true;
            break;
        case 'c':
            z = &c;
            opt_c = true;
            break;
        case 'd':
            z = &d;
            opt_d = true;
            break;
        case 'e':
            z = &e;
            opt_e = true;
            break;
        case 'f':
            z = &f;
            opt_f = true;
            break;
        case 'g':
            z = &g;
            opt_g = true;
            break;
        case 'h':
            z = &h;
            opt_h = true;
            break;
        case 'i':
            z = &i;
            opt_i = true;
            break;
        case 'j':
            z = &j;
            opt_j = true;
            break;
        case 'n': partitions = strtol(optarg, NULL, 10); break;
        case 'p':
            low = strtod(optarg, &pi);
            if (strcmp(pi, "pi") == 0) {
                low = M_PI;
            }
            if (strcmp(pi, "-pi") == 0) {
                low = -M_PI;
            }
            break;
        case 'q':
            high = strtod(optarg, &pi);
            if (strcmp(pi, "pi") == 0) {
                high = M_PI;
            }
            if (strcmp(pi, "-pi") == 0) {
                high = -M_PI;
            }
            break;

        case 'H':
            printf("Synopsis:\n Performs numerical integrations using composite Simpson's 1/3 "
                   "rule.\nUsage:\n./integrate OPTIONS\n\nOptions:\n-a:\n	sqrt(1 - "
                   "x^4)\n-b:\n	1/log(x)\n-c:\n	exp(-x^2)\n-d:\n	sin(x^2)\n-e:\n	"
                   "cos(x^2)\n-f:\n	log(log(x))\n-g:\n	sin(x)/x\n-h:\n	"
                   "exp(-x)/x\n-i:\n	exp(exp(x))\n-j:\n	sqrt(sin^2(x) + cos^2(x)\n-n "
                   "partitions:\n	Maximum number of partitions.\n-p low:\n	Set low "
                   "end of integral interval.\n-q high:\n	Set high end of the integral "
                   "interval.\n-h\n	Display program synopsis and usage\n");
            break;
        default: fprintf(stderr, "error\n"); return 1;
        }
    }
    if (opt_a == true) {
        printf("sqrt(1 - x^4), %lf, %lf, %ld\n", low, high, partitions);

        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_b == true) {
        printf("1/log(x), %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_c == true) {
        printf("e^(-x^2), %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_d == true) {
        printf("sin(x^2), %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_e == true) {
        printf("cos(x^2), %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_f == true) {
        printf("log(log(x)), %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_g == true) {
        printf("sin(x)/x, %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_h == true) {
        printf("(e^-x)/x, %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_i == true) {
        printf("e^e^x, %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    if (opt_j == true) {
        printf("sqrt(sin^2(x) + cos^2(x)), %lf, %lf, %ld\n", low, high, partitions);
        while (starting_n <= partitions) {
            printf("%ld, %.15f\n", starting_n, integrate(z, low, high, starting_n));
            starting_n += 2;
        }
    }
    return 0;
}
