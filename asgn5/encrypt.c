#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <string.h>
#include "numtheory.h"
#include <time.h>
#include <gmp.h>

#include "randstate.h"
#include "rsa.h"

int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pubfile;
    bool verbose = false;
    bool pb = false;
    while ((opt = (getopt(argc, argv, "i:o:n:vh"))) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                fprintf(stderr, "error opening file");
                return 0;
            }
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            if (outfile == NULL) {
                fprintf(stderr, "error opening file");
                return 0;
            }
            break;
        case 'n':
            pubfile = fopen(optarg, "r");
            if (pubfile == NULL) {
                fprintf(stderr, "error opening public file for reading");
                return 0;
            }
            pb = true;
            break;
        case 'v': verbose = true; break;
        case 'h':
            printf("Program Details\n");
            printf("	Encrypts a file using rsa encryption\n");
            printf("\nUSAGE\n");
            printf("-i	specifies the input file to encrypt (Default = stdin)\n");
            printf("-o	specifies the output file to encrypt to (Default = stdout)\n");
            printf("-n	specifies where to access the public key (Default = rsa.pub)\n");
            printf("-v	enables verbose output to stdout\n");
            printf("-h	shows program synopsis and usage\n\n");
            return 0;
        default:
            fprintf(stderr, "invalid input, type ./encrypt -h to see valid arguments\n");
            return 0;
        }
    }
    mpz_t n, e, s, user;
    mpz_inits(n, e, s, user, NULL);
    char username[32];

    if (pb == false) {
        pubfile = fopen("rsa.pub", "r");
        if (pubfile == NULL) {
            fprintf(stderr, "error opening public file");
            return 0;
        }
    }

    rsa_read_pub(n, e, s, username, pubfile);

    if (verbose == true) {
        gmp_printf("user = %s\n", username);
        size_t sizen = mpz_sizeinbase(n, 2);
        size_t sizee = mpz_sizeinbase(e, 2);
        size_t sizes = mpz_sizeinbase(s, 2);
        gmp_printf("(%d bits) s = %Zd\n", sizes, s);
        gmp_printf("(%d bits) n = %Zd\n", sizen, n);
        gmp_printf("(%d bits) e = %Zd\n", sizee, e);
    }

    mpz_set_str(user, username, 62); //verify message(user)

    if (rsa_verify(user, s, e, n) == false) { //if verify fails
        fprintf(stderr, "failed to verify signature\n");
        return 0;
    }

    rsa_encrypt_file(infile, outfile, n, e);

    mpz_clears(n, e, s, user, NULL);
}
