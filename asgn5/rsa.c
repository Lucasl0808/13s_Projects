#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdlib.h>
#include <inttypes.h>
#include "numtheory.h"
#include "randstate.h"

gmp_randstate_t state;

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t pbits = (random() % ((3 * nbits) / 4)) + (nbits / 4);
    uint64_t qbits = nbits - pbits; //qbits = remaining bits
    mpz_t lcm, gcd_pq, abs_pq, pq, p_1, q_1, gcd_e;
    mpz_inits(lcm, gcd_pq, abs_pq, pq, p_1, q_1, gcd_e, NULL);

    make_prime(p, pbits, iters); //p = prime number with p bits and iters iters
    make_prime(q, qbits, iters); //q = prime number with remaining bits and iters iters

    mpz_mul(n, q, p); //n = p * q
    //lcm = abs(p-1*q-1) / gcd(p-1,q-1)

    mpz_sub_ui(p_1, p, 1); //p_1 = p-1
    mpz_sub_ui(q_1, q, 1); //q_1 = q-1
    mpz_mul(pq, p_1, q_1); //pq = p-1 * q-1
    mpz_abs(abs_pq, pq); //abs_pq = abs(pq)
    gcd(gcd_pq, p_1, q_1); //gcd_pq = gcd(p-1, q-1)
    mpz_fdiv_q(lcm, abs_pq, gcd_pq); //lcm = abs_pq/gcd_pq
    mpz_urandomb(e, state, nbits); //initialize e to be random number from 0- nbits length
    mpz_add_ui(e, e, 5);
    gcd(gcd_e, e, lcm); //gcd_e = gcd(e, lcm)
    while (mpz_cmp_ui(gcd_e, 1) != 0) { // if gcd(e, lcm) is not 1 (not coprime)

        mpz_urandomb(e, state, nbits); //e = random from 0 - nbits length
        gcd(gcd_e, e, lcm); //gcd_e = gcd(e, lcm)

        //loop breaks if e coprime with lcm, and e will equal the suitable number
    }
    mpz_clears(lcm, gcd_pq, abs_pq, pq, p_1, q_1, gcd_e, NULL);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    gmp_fscanf(pbfile, "%s\n", username);
    fclose(pbfile);
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    //assume pbfile is open for writing
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    gmp_fprintf(pbfile, "%s\n", username);
    fclose(pbfile);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    //d = mod inverse(e, lcm)
    mpz_t p_1, q_1, pq, abs_pq, gcd_pq, lcm;
    mpz_inits(p_1, q_1, pq, abs_pq, gcd_pq, lcm, NULL);
    mpz_sub_ui(p_1, p, 1); //p_1 = p-1
    mpz_sub_ui(q_1, q, 1); //q_1 = q-1
    mpz_mul(pq, p_1, q_1); //pq = p-1 * q-1
    mpz_abs(abs_pq, pq); //abs_pq = abs(pq)
    gcd(gcd_pq, p_1, q_1); //gcd_pq = gcd(p-1, q-1)
    mpz_fdiv_q(lcm, abs_pq, gcd_pq); //lcm = abs(p-1 * q-1) / gcd(p-1,q-1)
    mod_inverse(d, e, lcm); //d = mod_inverse(e, lcm)
    mpz_clears(p_1, q_1, pq, abs_pq, gcd_pq, lcm, NULL);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    //assume pv file is open for writing
    //d = priv key
    //n = p * q
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
    fclose(pvfile);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    //assume pv file is open for reading
    gmp_fscanf(pvfile, "%Zx\n", n); // n = first line
    gmp_fscanf(pvfile, "%Zx\n", d); // d = next line
    fclose(pvfile);
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    //encrypt using powermod c = powermod(m, e, n)
    pow_mod(c, m, e, n);
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    //assume infile is open and outfile is open
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    uint64_t k = mpz_sizeinbase(n, 2);
    k -= 1;
    k /= 8;
    uint8_t *array = (uint8_t *) malloc(k * sizeof(uint8_t));
    array[0] = 0xFF;

    int j = 0;
    while ((j = fread(array + 1, 1, k - 1, infile)) > 0) {
        mpz_import(m, j, 1, sizeof(uint8_t), 1, 0, array);
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    free(array);
    fclose(outfile);
    fclose(infile);
    mpz_clears(m, c, NULL);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    //decrypt using power mod m = powermod(c, d, n)
    pow_mod(m, c, d, n);
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    size_t k = mpz_sizeinbase(n, 2);
    k -= 1;
    k /= 8;
    uint8_t *array = (uint8_t *) malloc(k * sizeof(uint8_t));
    while (!feof(infile)) { //(j = fwrite(array, 1, k-1, infile)) > 0){
        //mpz_t c, m;
        //mpz_inits(c, m, NULL);
        gmp_fscanf(infile, "%Zx\n", c); // c = read line in infile as hex
        size_t j = 0;
        rsa_decrypt(m, c, d, n);
        //we have n and d, and are creating c
        //store decrypted message to new variable m and export m
        //export stores bytes read into j, stores into array, takes value c and puts in in array
        mpz_export(array, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(array + 1, 1, j - 1, outfile);
    }
    fprintf(outfile, "\n");
    free(array);
    fclose(outfile);
    fclose(infile);
    mpz_clears(m, c, NULL);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    //s = m ^ d (mod n) = power mod
    pow_mod(s, m, d, n); //s = power mod(m = base, d = exponent, n = modulus)
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    //return true if temp t = m
    //t = power mod(s, e, n)
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n); //t = power mod (s, e, n)
    if (mpz_cmp(t, m) == 0) { // if t == m
        mpz_clear(t);
        return true;
    } else {
        mpz_clear(t);
        return false;
    }
}
