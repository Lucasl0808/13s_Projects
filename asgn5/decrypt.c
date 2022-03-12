#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>
#include "numtheory.h"
#include <gmp.h>

#include "rsa.h"

int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pvfile;
    bool verbose = false;
    bool pv = false;
    while ((opt = (getopt(argc, argv, "i:o:n:vh"))) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                fprintf(stderr, "error opening infile");
                return 0;
            }
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            if (outfile == NULL) {
                fprintf(stderr, "error opening outfile");
                return 0;
            }
            break;
        case 'n':
            pvfile = fopen(optarg, "r");
            if (pvfile == NULL) {
                fprintf(stderr, "error opening private key file");
                return 0;
            }
            pv = true;
            break;
        case 'v': verbose = true; break;
        case 'h':
            printf("Program Details\n");
            printf("	Decrypt message using RSA algorithm\n");
            printf("\nUSAGE\n");
            printf("-i	specifies input file to decrypt (Default = stdin)\n");
            printf("-o	specifies output file to decrypt to (Default = stdout)\n");
            printf("-n	specifies the file where the private key is located (Default = "
                   "rsa.priv)\n");
            printf("-v	enables verbose output\n");
            printf("-h	displays program details and usage\n");
            return 0;
        default:
            fprintf(stderr, "error, type ./decrypt -h for help on running program\n");
            return 0;
        }
    }
    if (pv == false) {
        pvfile = fopen("rsa.priv", "r");
        if (pvfile == NULL) {
            fprintf(stderr, "error opening private key file");
            return 0;
        }
    }

    mpz_t n, d;
    mpz_inits(n, d, NULL);

    rsa_read_priv(n, d, pvfile);

    if (verbose == true) {
        size_t sizen = mpz_sizeinbase(n, 2);
        size_t sized = mpz_sizeinbase(d, 2);
        gmp_printf("(%d bits) n = %Zd\n", sizen, n);
        gmp_printf("(%d bits) d = %Zd\n", sized, d);
    }

    rsa_decrypt_file(infile, outfile, n, d);

    mpz_clears(n, d, NULL);
    return 0;
}
