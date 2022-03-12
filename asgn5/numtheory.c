#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdlib.h>
#include <inttypes.h>
#include "randstate.h"

gmp_randstate_t state;

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t temp_a, temp_b;
    mpz_inits(temp_a, temp_b, NULL);
    mpz_set(temp_a, a); //temp a = a
    mpz_set(temp_b, b); //temp b = b
    while (mpz_cmp_ui(temp_b, 0) != 0) { //while b != 0
        mpz_t t;
        mpz_init_set(t, temp_b); //t = b
        mpz_mod(temp_b, temp_a, temp_b); //b = a % b
        mpz_set(temp_a, t); //a = t
        mpz_clear(t);
    }
    mpz_set(d, temp_a); //d = a
    mpz_clears(temp_a, temp_b, NULL);
}

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t v, p, temp_expo;
    mpz_inits(v, p, temp_expo, NULL);
    mpz_set_ui(v, 1); //v = 1
    mpz_set(p, base); //p = base
    mpz_set(temp_expo, exponent); //temporary exponent value
    while (mpz_cmp_ui(temp_expo, 0) > 0) {
        if (mpz_odd_p(temp_expo)) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, modulus);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, modulus);
        mpz_fdiv_q_ui(temp_expo, temp_expo, 2);
    }
    mpz_set(out, v);
    mpz_clears(v, p, temp_expo, NULL);
}

void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r, rp, t, tp;
    mpz_inits(r, rp, t, tp, NULL);
    mpz_set(r, n); //r = n
    mpz_set(rp, a); //rp = a
    mpz_set_ui(t, 0); //t = 0
    mpz_set_ui(tp, 1); //tp = 1
    while (mpz_cmp_ui(rp, 0) != 0) { //mpz_cmp_ui(rp, 0) < 0){        //while rp != 0
        mpz_t q, tmp_rp, tmp_r, tmp_t, tmp_tp, q_rp, q_tp;
        mpz_inits(q, tmp_rp, tmp_r, tmp_t, tmp_tp, q_rp, q_tp,
            NULL); //q, tmp_rp, tmp_r, tmp_t, tmp_tp = 0
        mpz_fdiv_q(q, r, rp); //q = floor div(r / rp)
        mpz_set(tmp_rp, rp); //tmp_rp = rp
        mpz_set(tmp_r, r); //tmp_r = r
        mpz_set(r, tmp_rp); //r = tmp_rp
        mpz_mul(q_rp, q, tmp_rp); //q_rp = q * tmp_rp
        mpz_sub(rp, tmp_r, q_rp); //rp = tmp_r - (q * rp)
        mpz_set(tmp_tp, tp); //tmp_tp = tp
        mpz_set(tmp_t, t); //tmp_t = t
        mpz_set(t, tmp_tp); //t = tmp_tp
        mpz_mul(q_tp, q, tmp_tp); //q_tp = q * tmp_tp
        mpz_sub(tp, tmp_t, q_tp); //tp = tmp_t - (q * tmp_t)
        mpz_clears(q, tmp_rp, tmp_r, tmp_t, tmp_tp, q_rp, q_tp, NULL);
    }
    if (mpz_cmp_ui(r, 1) > 0) { //if r > 1
        mpz_set_ui(i, 0); //i = 0
    }
    if (mpz_cmp_ui(t, 0) < 0) { //if t < 0
        mpz_add(t, t, n); //t = t + n
    }
    mpz_set(i, t); //i = t

    mpz_clears(r, rp, t, tp, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
    if (mpz_cmp_ui(n, 4) < 0) {
        return true;
    }
    if (mpz_even_p(n)) {
        return false; //return false if n is even
    }
    mpz_t temp, s, r;
    mpz_init_set(temp, n);
    mpz_inits(s, r, NULL); //s, r,x = 0

    mpz_set_ui(r, 1); //r =1
    mpz_sub_ui(temp, temp, 1); //temp = n-1
    while (mpz_cmp_ui(temp, 1) != 0) { //while temp != 1
        mpz_fdiv_q_ui(temp, temp, 2); //temp /= 2
        mpz_add_ui(s, s, 1); // s += 1
        if (mpz_odd_p(temp)) { //if temp %2 == 1
            mpz_set(r, temp); //r = temp
            break;
        }
    }
    for (uint64_t x = 1; x < iters; x += 1) {
        mpz_t a, y, ran_n, sub_s, two;
        mpz_inits(a, y, ran_n, sub_s, two, NULL); //y = 0, a = 0, ran_n = 0, sub_s = 0, two = 0
        mpz_sub_ui(ran_n, n, 1); //ran_n = n - 1
        mpz_sub_ui(sub_s, s, 1); //sub_s = s - 1
        mpz_set_ui(two, 2); //two = 2
        mpz_urandomm(a, state, ran_n); //a = random from 2 -> n-2
        mpz_add_ui(a, a, 1); //set lower bound of a to 2
        pow_mod(y, a, r, n); // y = power mod(a, r, n)
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, ran_n) != 0) { //if y != 1 and y != n-1
            mpz_t j;
            mpz_init_set_ui(j, 1); //j = 1
            while (
                mpz_cmp(j, sub_s) <= 0 && mpz_cmp(y, ran_n) != 0) { //while j <= s - 1 and y != n-1
                pow_mod(y, y, two, n); //y = power mod(y, 2, n)

                if (mpz_cmp_ui(y, 1) == 0) { //if y == 1
                    mpz_clears(j, a, y, ran_n, sub_s, two, s, r, temp, NULL);
                    return false; //return false
                }
                mpz_add_ui(j, j, 1); // j += 1
            }
            if (mpz_cmp(y, ran_n) != 0) { //if y != n-1
                mpz_clears(j, a, y, ran_n, sub_s, two, s, r, temp, NULL);
                return false; //return false
            }
            mpz_clear(j);
        }
        mpz_clears(a, y, ran_n, sub_s, two, NULL);
    }
    mpz_clears(s, r, temp, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t bitcount, prime, ran_bits, one;
    mpz_inits(bitcount, prime, ran_bits, one, NULL);
    mpz_set_ui(bitcount, bits); //bitcount = min bits
    mpz_set_ui(one, 1);
    mpz_mul_2exp(bitcount, one, bits); //bitcount = 2^bits
    uint64_t rand = bits - 1; //rand = bits -1
    mpz_set(p, bitcount); //p = 2^bits
    mpz_set_ui(ran_bits, rand); //ran_bits = bits - 1
    mpz_urandomb(prime, state, bits - 1); //prime = random number from 0 -> 2^bits -1
    mpz_add(p, p, prime); //p = 2^bits + random number from 0 -> 2^bits -1i
    while (is_prime(p, iters) == false) {
        mpz_set(p, bitcount); //p = 2 ^ bits
        mpz_urandomb(prime, state, bits - 1); //prime = random from 0 -> 2^bits-1
        mpz_add(p, p, prime); //p = 2^bits + random number from 0->2^bits-1
    }
    mpz_clears(bitcount, prime, ran_bits, one, NULL);
}
