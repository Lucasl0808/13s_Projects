#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

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


