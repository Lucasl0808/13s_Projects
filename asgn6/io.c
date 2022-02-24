#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "defines.h"
#include "code.h"

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

//assume file is open and file descriptor infile and outfile are already defined
int read_bytes(int infile, uint8_t *buf, int nbytes){
	//read() writes bytes read from file descriptor into buffer
	//buffer exists outside of this function
	//stop calling read() when nbytes are read or read returns 0
	//inside loop call to read(), track how many bytes are read, increment buffer by that many to not override 
	//string and continue writing to buffer
	//subtract bytes read from nbytes to not read the full block of bytes again
	
	int temp_bytes = nbytes;
	int total_bytes_read = 0;
	while(total_bytes_read != nbytes){
		int curr_read = read(infile, buf + total_bytes_read, temp_bytes);	//curr_read = bytes read from infile. Read bytes into buffer + amount of bytes read
		total_bytes_read += curr_read;
		temp_bytes -= curr_read;
		if(curr_read == 0){	//if no more bytes are read
			break;
		}
	}
	bytes_read += total_bytes_read;
	return total_bytes_read;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes){
	int temp_bytes = nbytes;
	int total_bytes_written = 0;
	while(total_bytes_written != nbytes){
		int curr_write = write(outfile, buf + total_bytes_written, temp_bytes);
		total_bytes_written += curr_write;
		temp_bytes -= curr_write;
		if(curr_write == 0){	//if no more bytes to write
			break;
		}
	}
	bytes_written += total_bytes_written;
	return total_bytes_written;
}


bool read_bit(int infile, uint8_t *bit){

	static int count = 0;
	static int index = 0;
	static uint8_t buffer[BLOCK];
	if(count == 0 && index == 0){
		read_bytes(infile, buffer, BLOCK);	//if we need to fill up the empty buffer, fill it with bytes from infile
		//printf("buffer = %" PRIu8 "\n", buffer[index]);
	}
	
	//printf("buffer = %" PRIu8 "\n", buffer[index]);

	if(buffer[index] % 2 == 1){	//if rightmost bit in buffer index is 1
		*bit = 1;	//store 1 into bit pointer
	}
	else{
		*bit = 0;	//if rightmost bit is 0 (odd number) then return 0 to bit pointer
	}

	buffer[index]  = buffer[index]>> 1;	//shift byte right by 1 after reading current bit
	count += 1;	//increment count to keep track of how many bits we've read from a certain byte

	if(count == 8){ //if we have reached the end of a byte, reset count to 0 and move to the next byte
		index += 1;
		count = 0;
	}

	if(count == 0 && index == BLOCK){       //if we have reached the end of the buffer, reset index and count to the start
		index = 0;
		count = 0;
		printf("reached end of buffer");
		return false;
	}
	//printf("buffer = %" PRIu8 "\n", buffer[index]);
	return true;
}

static uint8_t write_buffer[BLOCK];
static int write_index = 0;

void write_code(int outfile, Code *c){
	//for each bit in Code c, write it to the static buffer (outside function)
	//write_bytes to outfile using static buffer once it is full
	//otherwise fill up buffer by reading in code c
	//code_get_bit(c, uint32_t i) where i is the bit number returns its value as a bool
	//while code c is not 0, add rightmost bit to buffer 
	
	int index = 0;	//c->bit[index] = index in bit array
	uint32_t bit_location = 0;	//bit_location = index to get bit in code
	uint32_t exponent = 1;	//stores value to use as: 2^exponent (starts at 0)
	uint32_t bits_written = 0;

	while(bits_written != c->top){	//while number of bits written != c->top (c->top = bits pushed to code)
		if(code_get_bit(c, bit_location)){	//if rightmost bit is 1 (true)
			write_buffer[index] += exponent;	//add bit (2^exponent) to buffer[index]
			bits_written += 1;
		}
		if(code_get_bit(c, bit_location) == false){	//if rightmost bit is 0 (false)
			//dont write anything to buffer
			bits_written += 1;
		}
		exponent *= 2;	//increase value of exponent by 1
		bit_location += 1;	//get the bit in the next bit location
		if(bit_location % 8 == 0){	//if bit location is  a multiple of 8 (goes into next index)
			write_index += 1;	//start writing into the next index
		}
		if(index == BLOCK){	//if we finished writing to the last index in write_buffer
			write_bytes(outfile, write_buffer, BLOCK);	//write BLOCK bytes of write_buffer to outfile
			for(int x = 0; x < BLOCK; x += 1){
				write_buffer[x] = 0;	//clear buffer after writing to outfile
				write_index = 0; 	//reset index after clearing buffer to write to the beginning
			}
		}
	}

}

int main(void){
	int fd = open("test.txt", O_RDONLY);
	printf("fd = %d\n", fd);
	
	uint8_t l = 0;
	//uint8_t a = 0;
	//uint8_t b = 0;
	//uint8_t c = 0;
	
	read_bit(fd, &l);
	read_bit(fd, &l);
	read_bit(fd, &l);
	read_bit(fd, &l);
	read_bit(fd, &l);
	read_bit(fd, &l);
	read_bit(fd, &l);
	read_bit(fd, &l);
	read_bit(fd, &l);
	read_bit(fd, &l);
	
	/*
	int count = 0;
	while(read_bit(fd, &l)){
		if(l == 1){
			printf("%" PRIu8 "\n", l);
		}
		count += 1;
	}
	printf("%d\n", count);
	*/
	printf("l = %" PRIu8 "\n", l);
	close(fd);

	return 0;
}

