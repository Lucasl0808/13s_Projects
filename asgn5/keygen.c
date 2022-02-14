#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>
#include "numtheory.h"
#include <time.h>
#include <gmp.h>

#include "randstate.h"
#include "rsa.h"
gmp_randstate_t state;

int main(int argc, char **argv){
	int opt = 0;
	uint64_t min_bits = 256;
	uint64_t iters = 50;
	FILE *pbfile;	//specify rsa.pub later
	FILE *pvfile;	//specify rsa.priv later
	uint64_t seed = time(NULL);
	bool verbose = false;
	bool pb = false;
	bool pv = false;
	while((opt = (getopt(argc, argv, "b:i:n:d:s:vh"))) != -1){
		switch(opt){
		case 'b':
			min_bits = strtol(optarg, NULL, 10);
			break;
		case 'i':
			iters = strtol(optarg, NULL, 10);
			break;
		case 'n':
			pbfile = fopen(optarg, "w");
			if(pbfile == NULL){
				fprintf(stderr, "error with public file");
				return 0;
			}
			pb = true;
			break;
		case 'd':
			pvfile = fopen(optarg, "w");
			if(pvfile == NULL){
				fprintf(stderr, "error with private file");
				return 0;
			}
			pv = true;
			break;
		case 's':
			seed = strtol(optarg, NULL, 10);
			break;
		case 'v':
			verbose = true;
			break;
		case 'h':
			printf("Program Details:\n");
			printf("generates random public and private keys with specified numbers of bits\n");
			printf("USAGE\n");
			printf("-b	specifies minimum bits for modulus n\n");
			printf("-i	specifies iterations for testing prime numbers (default = 50)\n");
			printf("-n pbfile	specifies where to print public key(default = rsa.pub)\n");
			printf("-d pvfile	specifies where to print private key(default = rsa.priv)\n");
			printf("-s	specifies the random seed(default = time(NULL))\n");
			printf("-v	enables verbose output to stdout\n");
			printf("-h	displays usage for program\n");
			break;
		default:
			fprintf(stderr, "type ./keygen -h for help and type valid arguments\n");
			return 0;
		}
	}

	randstate_init(seed);
	mpz_t p,q,n,e,d,name,s;
	mpz_inits(p,q,n,e,d,name,s, NULL);
	if(pb == false){
		pbfile = fopen("rsa.pub", "w");
		if(pbfile == NULL){
			fprintf(stderr,"error opening file");
			return 0;
		}
	}
	if(pv == false){
		pvfile = fopen("rsa.priv", "w");
		if(pvfile == NULL){
			fprintf(stderr,"error opening file");
			return 0;
		}
	}
	int fd = fileno(pvfile);
	fchmod(fd, 0600); // set file perms to 0600
	
	rsa_make_pub(p, q, n, e, min_bits, iters);
	rsa_make_priv(d, e, p, q);

	char *username;
	username = getenv("USER");

	mpz_set_str(name, username, 62);

	rsa_sign(s, name, d, n);

	rsa_write_pub(n,e,s, username, pbfile);
	rsa_write_priv(n,d,pvfile);

	if(verbose == true){
		gmp_printf("user = %s\n", username);
		size_t sizes = mpz_sizeinbase(s, 2);
		size_t sizep = mpz_sizeinbase(p, 2);
		size_t sizeq = mpz_sizeinbase(q, 2);
		size_t sizen = mpz_sizeinbase(n, 2);
		size_t sizee = mpz_sizeinbase(e, 2);
		size_t sized = mpz_sizeinbase(d, 2);
		gmp_printf("(%d bits) s = %Zd\n", sizes,s);
		gmp_printf("(%d bits) p = %Zd\n", sizep,p);
		gmp_printf("(%d bits) q = %Zd\n", sizeq,q);
		gmp_printf("(%d bits) n = %Zd\n", sizen,n);
		gmp_printf("(%d bits) e = %Zd\n", sizee,e);
		gmp_printf("(%d bits) d = %Zd\n", sized,d);
		
		//printf("works");
	}


	//fclose(pbfile);
	//fclose(pvfile);
	randstate_clear();
	mpz_clears(p,q,n,e,d,name,s,NULL);

}

