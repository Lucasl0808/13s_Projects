#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "defines.h"
#include "code.h"

Code code_init(void){
	Code c;
	c.top = 0;
	for(int i = 0; i < MAX_CODE_SIZE; i += 1){
		c.bits[i] = 0;
	}
	return c;
}

uint32_t code_size(Code *c){
	return c->top;
}

bool code_empty(Code *c){
	if(code_size(c) == 0){
		return true;
	}
	else{
		return false;
	}
}

bool code_full(Code *c){
	if(code_size(c) == ALPHABET){
		return true;
	}
	else{
		return false;
	}
}

bool code_set_bit(Code *c, uint32_t i){
	if(i > ALPHABET || i < 0){
		return false;
	}
	uint32_t temp = 8;
        uint32_t tempi = 0;
        uint32_t tempr = 0;
        uint8_t and = 1;
        uint8_t tempc = c->bits[i];     //temporary value holding number at c->bits[i]
        tempi = i / temp;       //gets array location
        tempr = i % temp;       //gets array position
        for(uint32_t x = 0; x < tempr; x += 1){
                and *= 2;
        }
        //and is value 2^array position
        tempc = tempc | and;   //tempc = c->bits[i] where location of i is zero makes sure there is a 1 in correct position


        c->bits[tempi] = c->bits[tempi] | tempc;        //make a copy of c->bits[i] using | to add one to desired bit
	return true;
}

bool code_clr_bit(Code *c, uint32_t i){
	if(i > ALPHABET || i < 0){
		return false;
	}
	uint32_t temp = 8;
	uint32_t tempi = 0;
	uint32_t tempr = 0;
	uint8_t and = 1;
	uint8_t tempc = c->bits[i];	//temporary value holding number at c->bits[i]
	tempi = i / temp;	//gets array location
	tempr = i % temp;	//gets array position
	for(uint32_t x = 0; x < tempr; x += 1){
		and *= 2;
	}
	//and is value 2^array position
	tempc = 255 - and;	//tempc = 8 bits of 1's where desired position is set to 0 to ensure that when using & operator the desired bit is set to 0


	c->bits[tempi] = c->bits[tempi] & tempc;	//make a copy of c->bits[i] using & to zero out desired bit
	return true;
}

bool code_get_bit(Code *c, uint32_t i){
	if(i > ALPHABET || i < 0){
		return false;
	}
	//uint32_t temp = 8;
        uint32_t tempi = 0;
        uint32_t tempr = 0;
        uint8_t tempc = c->bits[i];     //temporary value holding number at c->bits[i]
        tempi = i / 8;       //gets array location
        tempr = i % 8;       //gets array position
	tempc = c->bits[tempi] >> tempr;	//tempc shifts right by the position of the desired number. Puts desired bit at the 0th index
	if(tempc % 2 == 1){	//if shifted number is odd (desired number is 1)
		return true;
	}
	else{
		return false;
	}

}

bool code_push_bit(Code *c, uint8_t bit){
	if(code_full(c)){
		return false;
	}
	if(bit == 0){	//if bit is 0 then increment the top only (array initialized as 0)
		c->top += 1;
		return true;
	}
	if(bit > 0){	//if bit is anything above 0 then set top bit to 1 and increment top by 1
		if(code_set_bit(c, c->top) == false){
			return false;
		}
		else{
			c->top += 1;
			return true;
		}
	}

	return true;
}

bool code_pop_bit(Code *c, uint8_t *bit){
	if(code_empty(c)){
		return false;
	}
	if(code_get_bit(c, c->top-1)){	//if top bit is 1
		*bit = 1;	//set bit = 1
		code_clr_bit(c, c->top-1);	//clear top bit
		c->top -=1;	//decrement top
		return true;
	}
	else if(code_get_bit(c, c->top-1) == false){	//if top bit is 0
		*bit = 0;	//bit = 0
		code_clr_bit(c, c->top-1);	//clear top bit
		c->top -=1;	//decrement top.
		return true;
	}
	return true;


}


void code_print(Code *c){
	
	for(uint32_t i =0; i < c->top; i += 1){
		if(code_get_bit(c, i) == false){
			printf("0");
		}
		if(code_get_bit(c, i) == true){
			printf("1");
		}
		//printf("%"PRIu8 " ", c->bits[i]);

	}
	

	printf("top = %" PRIu32 "\n", c->top);
}
/*
int main(void){
	Code c = code_init();
	//code_push_bit(&c, 1);
	//code_push_bit(&c, 1);
	
	//uint8_t a = 0;
	//uint8_t b = 0;
	//code_pop_bit(&c, &a);
	//code_pop_bit(&c, &b);
	//printf("a = %" PRIu8 "\n", a);
	//printf("b = %" PRIu8 "\n", b);
	//printf("%" PRIu32 "\n", c.top-1);
	//if(code_get_bit(&c, 1)){
	//	printf("top bit is 1");
	//	return 1;
	//}

        //if(code_get_bit(&c,0)){
        //        printf("bit = true\n");
        //}
        //else{
        //        printf("bit = false\n");
        //}

	//if(code_clr_bit(&c, c.top) == false){
	//	return 1;
	//}
	
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);

	if(code_get_bit(&c, c.top-1)){
		printf("index is true\n");
	}

	//code_push_bit(&c, 255);
	code_print(&c);
	uint8_t a = 0;
	uint8_t b = 0;
	uint8_t f = 0;
	uint8_t d = 0;
	code_pop_bit(&c, &a);
	code_pop_bit(&c, &b);
	code_pop_bit(&c, &f);
	code_pop_bit(&c, &d);
	printf("first = %" PRIu8 ", second = %"PRIu8 ", third = %"PRIu8 "fourth = %"PRIu8 "\n", a, b, f, d);
	
	code_print(&c);
	return 1;
}
*/
