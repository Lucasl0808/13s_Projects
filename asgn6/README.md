#Assignment 6 - Huffman Coding
This assignment uses huffman tree logic to compress files through encoding and decoding

##Compile/Build
make all = creates encode and decode executables, as well as all the object files for each .c file<br>
make encode = creates the encode executable<br>
make decode = creates the decode executable<br>
make clean = removes all created object files and binary executable files<br>

##Running Code
run command: ./encode -h for info on how to run the encoder<br>
run command: ./decode -h for info on how to run the encoder<br>

##Issues/Errors to report
If you call for a verbose output on the decoder without an outfile, it will cause an error and not print correctly.<br>
Otherwise, giving the decoder an outfile will not cause this problem.


