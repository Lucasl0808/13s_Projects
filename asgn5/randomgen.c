#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdlib.h>
#include <inttypes.h>
#include "randstate.h"
gmp_randstate_t state;

void randstate_init(uint64_t seed){
	srandom(seed);
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, seed);
}

void randstate_clear(void){
	gmp_randclear(state);
}

void gcd(mpz_t d, mpz_t a, mpz_t b){
	mpz_t temp_a, temp_b;
	mpz_inits(temp_a, temp_b, NULL);
	mpz_set(temp_a, a);	//temp a = a
	mpz_set(temp_b, b);	//temp b = b
	while(mpz_cmp_ui(temp_b, 0) != 0){ //while b != 0
		mpz_t t;
		mpz_init_set(t, temp_b);	//t = b
		mpz_mod(temp_b, temp_a, temp_b);	//b = a % b
		mpz_set(temp_a, t);		//a = t
		mpz_clear(t);
	}
	mpz_set(d, temp_a);	//d = a
	mpz_clears(temp_a, temp_b, NULL);
}

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus){
	mpz_t v, p, temp_expo;	
	mpz_inits(v, p, temp_expo, NULL);	
	mpz_set_ui(v, 1);	//v = 1
	mpz_set(p, base);	//p = base
	mpz_set(temp_expo, exponent);	//temporary exponent value
	while(mpz_cmp_ui(temp_expo, 0) > 0){
		if(mpz_odd_p(temp_expo)){
			mpz_mul(v, v, p);
			mpz_mod(v, v, modulus);
		}
		mpz_mul(p, p, p);
		mpz_mod(p, p, modulus);
		mpz_fdiv_q_ui(temp_expo, temp_expo, 2);
	}
	mpz_set(out, v);
	mpz_clears(v,p,temp_expo, NULL);
}

void mod_inverse(mpz_t i, mpz_t a, mpz_t n){
	mpz_t r, rp, t, tp;
	mpz_inits(r, rp, t, tp, NULL);
	mpz_set(r, n);		//r = n
	mpz_set(rp, a);		//rp = a
	mpz_set_ui(t, 0);	//t = 0
	mpz_set_ui(tp, 1);	//tp = 1
	while(mpz_cmp_ui(rp, 0) != 0){ //mpz_cmp_ui(rp, 0) < 0){	//while rp != 0
		mpz_t q, tmp_rp, tmp_r, tmp_t, tmp_tp, q_rp, q_tp;
		mpz_inits(q, tmp_rp, tmp_r, tmp_t, tmp_tp, q_rp, q_tp, NULL);	//q, tmp_rp, tmp_r, tmp_t, tmp_tp = 0
		mpz_fdiv_q(q, r, rp);	//q = floor div(r / rp)
		mpz_set(tmp_rp, rp);	//tmp_rp = rp
		mpz_set(tmp_r, r);	//tmp_r = r
		mpz_set(r, tmp_rp);	//r = tmp_rp
		//mpz_mul(rp, q, tmp_rp); //rp = q * tmp_rp
		mpz_mul(q_rp, q, tmp_rp); //q_rp = q * tmp_rp
		mpz_sub(rp, tmp_r, q_rp); //rp = tmp_r - (q * rp)
		mpz_set(tmp_tp, tp);	//tmp_tp = tp
		mpz_set(tmp_t, t);	//tmp_t = t
		mpz_set(t, tmp_tp);	//t = tmp_tp
		//mpz_mul(tp, q, tmp_tp);	//tp = q * tmp_t
		mpz_mul(q_tp, q, tmp_tp);	//q_tp = q * tmp_tp
		mpz_sub(tp, tmp_t, q_tp);	//tp = tmp_t - (q * tmp_t)
		mpz_clears(q, tmp_rp, tmp_r, tmp_t, tmp_tp, q_rp, q_tp, NULL);
	}
	if(mpz_cmp_ui(r, 1) > 0){	//if r > 1
		mpz_set_ui(i, 0);	//i = 0
	}
	if(mpz_cmp_ui(t, 0) < 0){	//if t < 0
		mpz_add(t, t, n);	//t = t + n
	}
	mpz_set(i, t);	//i = t


	mpz_clears(r, rp, t, tp, NULL);
}
bool is_prime(mpz_t n, uint64_t iters){
	if(mpz_cmp_ui(n, 4) < 0){
		return true;
	}
	if(mpz_even_p(n)){
		return false;	//return false if n is even
	}
	mpz_t temp, s, r;
	mpz_init_set(temp, n);
	mpz_inits(s, r, NULL);			//s, r,x = 0
	//mpz_add_ui(test, test,iters);
	//gmp_printf("test iters = %Zd\n",mpz_cmp_ui(x, iters) < 0);
	mpz_set_ui(r, 1);			//r =1
	mpz_sub_ui(temp, temp, 1);			//temp = n-1
	//gmp_printf("s = %Zd, r = %Zd, temp = %Zd\n",s,r,temp);
	while(mpz_cmp_ui(temp,1) != 0){		//while temp != 1
		mpz_fdiv_q_ui(temp, temp, 2);	//temp /= 2
		mpz_add_ui(s, s, 1); 		// s += 1
		if(mpz_odd_p(temp)){		//if temp %2 == 1
			mpz_set(r, temp);	//r = temp
			break;
		}
	}
	//gmp_printf("s = %Zd, r = %Zd\n", s, r);
	//for(mpz_set_ui(x, 1); mpz_cmp_ui(x, iters) < 0; mpz_add_ui(x, x, 1)){	//for(x = 1; x < iters; x+=1) FIX LOOP CONDITIONALS, SEGMENTATION ERROR
	for(uint64_t x = 1; x < iters; x += 1){
		mpz_t a, y, ran_n, sub_s, two;
		mpz_inits(a, y, ran_n, sub_s, two, NULL); //y = 0, a = 0, ran_n = 0, sub_s = 0, two = 0
		mpz_sub_ui(ran_n, n, 1);	//ran_n = n - 1
		mpz_sub_ui(sub_s, s, 1);	//sub_s = s - 1
		mpz_set_ui(two, 2);		//two = 2
		mpz_urandomm(a, state, ran_n);	//a = random from 2 -> n-2
		mpz_add_ui(a,a, 1);		//set lower bound of a to 2
		//gmp_printf("a = %Zd\n", a);
		pow_mod(y, a, r, n);		// y = power mod(a, r, n)
		//gmp_printf("y = %Zd\n", y);
		if(mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, ran_n) != 0){	//if y != 1 and y != n-1
			mpz_t j;
			mpz_init_set_ui(j, 1);	//j = 1
			while(mpz_cmp(j, sub_s) <= 0 && mpz_cmp(y, ran_n) != 0){	//while j <= s - 1 and y != n-1
				pow_mod(y, y, two, n);		//y = power mod(y, 2, n)
				//gmp_printf(" y after second powmod = %Zd, %Zd, %Zd\n", y, two, n);
				if(mpz_cmp_ui(y, 1) == 0){	//if y == 1
					mpz_clears(j, a,y,ran_n,sub_s,two,s,r,temp,NULL);
					return false;		//return false
				}
				mpz_add_ui(j, j, 1);		// j += 1
			}
			if(mpz_cmp(y, ran_n) != 0){		//if y != n-1
				mpz_clears(j,a,y,ran_n,sub_s, two, s, r, temp, NULL);
				return false;			//return false
			}
			mpz_clear(j);
		}
		mpz_clears(a, y, ran_n, sub_s, two, NULL);

	}
	mpz_clears(s, r, temp,NULL);
	return true;
}
void make_prime(mpz_t p, uint64_t bits, uint64_t iters){
	mpz_t bitcount, prime, ran_bits, one;
	mpz_inits(bitcount, prime, ran_bits, one, NULL);
	mpz_set_ui(bitcount, bits);	//bitcount = min bits
	mpz_set_ui(one, 1);
	mpz_mul_2exp(bitcount, one, bits); //bitcount = 2^bits
	uint64_t rand = bits - 1;	//rand = bits -1
	mpz_set(p, bitcount);	//p = 2^bits
	mpz_set_ui(ran_bits, rand);	//ran_bits = bits - 1
	mpz_urandomb(prime, state, bits-1);	//prime = random number from 0 -> 2^bits -1
	mpz_add(p, p, prime);	//p = 2^bits + random number from 0 -> 2^bits -1i
	while(is_prime(p, iters) == false){
		mpz_set(p, bitcount);	//p = 2 ^ bits
		mpz_urandomb(prime, state, bits-1);	//prime = random from 0 -> 2^bits-1
		mpz_add(p, p, prime);	//p = 2^bits + random number from 0->2^bits-1
	}
	mpz_clears(bitcount, prime, ran_bits, one, NULL);

}

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters){
	uint64_t pbits = (random() % ((3 * nbits)/4)) + (nbits / 4);
	uint64_t qbits = nbits - pbits;	//qbits = remaining bits
	mpz_t lcm, gcd_pq, abs_pq, pq, p_1, q_1, gcd_e;
	mpz_inits(lcm, gcd_pq, abs_pq, pq, p_1, q_1, gcd_e, NULL);

	make_prime(p, pbits, iters);	//p = prime number with p bits and iters iters
	make_prime(q, qbits, iters);	//q = prime number with remaining bits and iters iters

	mpz_mul(n, q, p);	//n = p * q
	//lcm = abs(p-1*q-1) / gcd(p-1,q-1)
	
	mpz_sub_ui(p_1, p, 1);	//p_1 = p-1
	mpz_sub_ui(q_1, q, 1);	//q_1 = q-1
	mpz_mul(pq, p_1, q_1);	//pq = p-1 * q-1
	mpz_abs(abs_pq, pq);	//abs_pq = abs(pq)
	gcd(gcd_pq, p_1, q_1);	//gcd_pq = gcd(p-1, q-1)
	mpz_fdiv_q(lcm, abs_pq, gcd_pq);	//lcm = abs_pq/gcd_pq
	printf("nbits = %" PRIu64"\n", nbits);
	mpz_urandomb(e, state, nbits);	//initialize e to be random number from 0- nbits length
	mpz_add_ui(e, e, 5);
	gcd(gcd_e, e, lcm);	//gcd_e = gcd(e, lcm)
	while(mpz_cmp_ui(gcd_e, 1) != 0){	// if gcd(e, lcm) is not 1 (not coprime)

		mpz_urandomb(e, state, nbits);	//e = random from 0 - nbits length
		gcd(gcd_e, e, lcm);	//gcd_e = gcd(e, lcm) 
		
		//loop breaks if e coprime with lcm, and e will equal the suitable number
	}
	mpz_clears(lcm, gcd_pq, abs_pq, pq, p_1, q_1, gcd_e,NULL);
}

//make read pub

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile){
	//assume pbfile is open for writing
	gmp_fprintf(pbfile, "%Zx\n", n);
	gmp_fprintf(pbfile, "%Zx\n", e);
	gmp_fprintf(pbfile, "%Zx\n", s);
	gmp_fprintf(pbfile, "%s\n", username);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q){
	//d = mod inverse(e, lcm)
	mpz_t p_1, q_1,pq, abs_pq, gcd_pq, lcm;
	mpz_inits(p_1, q_1, pq, abs_pq, gcd_pq, lcm, NULL);
	mpz_sub_ui(p_1, p, 1);  //p_1 = p-1
	mpz_sub_ui(q_1, q, 1);  //q_1 = q-1
	mpz_mul(pq, p_1, q_1);  //pq = p-1 * q-1
	mpz_abs(abs_pq, pq);    //abs_pq = abs(pq)
	gcd(gcd_pq, p_1, q_1);  //gcd_pq = gcd(p-1, q-1)
	mpz_fdiv_q(lcm, abs_pq, gcd_pq);	//lcm = abs(p-1 * q-1) / gcd(p-1,q-1)
	mod_inverse(d, e, lcm);	//d = mod_inverse(e, lcm)
	mpz_clears(p_1, q_1, pq, abs_pq, gcd_pq, lcm, NULL);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile){
	//assume pv file is open for writing
	//d = priv key
	//n = p * q
	gmp_fprintf(pvfile, "%Zx\n", n);
	gmp_fprintf(pvfile, "%Zx\n", d);
	fclose(pvfile);
}

//make read private

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n){
	//encrypt using powermod c = powermod(m, e, n)
	power_mod(c, m, e, n);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n){
	//decrypt using power mod m = powermod(c, d, n)
	power_mod(m, c, d, n);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n){
	//s = m ^ d (mod n) = power mod
	power_mod(s, m, d, n);	//s = power mod(m = base, d = exponent, n = modulus)
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n){
	//return true if temp t = m
	//t = power mod(s, e, n)
	mpz_t t;
	mpz_init(t);
	power_mod(t, s, e, n);	//t = power mod (s, e, n)
	if(mpz_cmp(t, m) == 0){	// if t == m
		mpz_clear(t);
		return true;
	}
	else{
		mpz_clear(t);i
		return false;
	}
}

//MAKE ENCRYPT/DECRYPT FILE FUNCTIONS

int main(void){
	
	randstate_init(123456);
	//gmp_randstate_t state;
	//gmp_randinit_mt(state);
	//gmp_randseed_ui(state, 10);
	//mpz_t x,z,y;
	//mpz_inits(x,z,y,NULL);
	//make_prime(z, 31, 50);
	//gmp_printf("prime z = %Zd\n", z);
	//mpz_set_ui(y, 3585261011236632);
	//mpz_set_ui(z, 79);
	//mpz_set_ui(z, 65537);
	/*for(int i = 0; i < 100; i +=1){
		mpz_add_ui(z, z, 1); //z = z+1
		printf("is prime = %d\n",is_prime(z, 50));
	}*/
	//gcd(x, y, z);
	//mod_inverse(x, y, z);
	//pow_mod(x,y,z,a);
	//gmp_printf("gcd = %Zd\n",x);
	//printf("z is prime = %d\n", is_prime(z, 50));
	
	mpz_t p,q,n,e, x;
	mpz_inits(p,q,n,e,x,NULL);
	rsa_make_pub(p,q,n,e,52, 50);
	gmp_printf("q = %Zd, p = %Zd, n = %Zd, e = %Zd\n", q, p, n, e);
	rsa_make_priv(x, e, p, q);
	gmp_printf("priv = %Zd\n", x);
	randstate_clear();
	mpz_clears(q,p,n,e,x,NULL);
	

	//randstate_clear();
	//mpz_clears(x,z,y,NULL);
}
