#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "huffman.h"
#include "header.h"
#include "defines.h"
#include "node.h"
#include "io.h"
#include "pq.h"
#include "stack.h"

int main(int argc, char **argv) {
    int opt = 0;
    int infile = 0; //default infile = stdin
    int outfile = 1; //default outfile = stdout
    bool verbose = false;
    while ((opt = (getopt(argc, argv, "hi:o:v"))) != -1) {
        switch (opt) {
        case 'h':
            fprintf(stderr, "Synopsis\n");
            fprintf(stderr, "       Uses Huffman encoding and decoding to compress files and "
                            "decode/encode messages\n");
            fprintf(stderr, "\nUSAGE\n");
            fprintf(stderr, "-h     Prints out program synopsis/usage details\n");
            fprintf(stderr, "-i infile      specify input file to use Huffman Decoding on\n");
            fprintf(stderr, "-o outfile     specify output file to write decompressed file from "
                            "huffman decoding to\n");
            fprintf(stderr, "-v     prints file compression statistics to stderr. Prints "
                            "compressed file size, decompressed file size, and space saving\n");
            return 1;
        case 'i':
            infile = open(optarg, O_RDONLY); //infile = specified file for reading
            if (infile == -1) {
                fprintf(stderr, "error opening infile\n");
                return 0;
            }
            break;
        case 'o':
            outfile = open(optarg, O_RDWR | O_CREAT | O_TRUNC);
            if (outfile == -1) {
                fprintf(stderr, "error opening outfile\n");
                return 0;
            }
            break;
        case 'v': verbose = true; break;
        default:
            fprintf(stderr, "invalid input, try again with -h to see valid arguments\n");
            return 0;
        }
    }
    struct stat infile_stat;
    fstat(infile, &infile_stat);
    Header h;

    read_bytes(infile, (uint8_t *) &h, sizeof(h)); //read header from infile

    if (h.magic != MAGIC) { //if header's magic number is not equal to MAGIC in defines.h
        fprintf(stderr, "magic number does not match\n");
        return 0;
    }

    fchmod(outfile, h.permissions);
    //make uint8 array tree_dump[tree_size] and read dumped tree into it
    uint8_t tree_dump[h.tree_size];

    read_bytes(infile, tree_dump, h.tree_size);
    //read tree_size bytes given by header into tree_dump array

    Node *root = rebuild_tree(h.tree_size, tree_dump);
    //root of the tree is the root of the rebuilt tree using tree_dump

    Node *temp_root = root; //make a temp root to iterate through the tree

    uint8_t bit = 0;
    uint64_t num_symbols = 0;
    while (num_symbols != h.file_size) {

        read_bit(infile, &bit);
        if (bit == 0) {
            temp_root = temp_root->left;
        } else {
            temp_root = temp_root->right;
        }

        if (temp_root->left == NULL && temp_root->right == NULL) {
            write_bytes(outfile, &(temp_root->symbol), 1);
            num_symbols += 1;
            temp_root = root;
        }
    }
    struct stat outfile_stat;
    fstat(outfile, &outfile_stat);

    if (verbose == true) {
        fprintf(stderr, "compressed file size = %lu bytes\n", infile_stat.st_size);
        fprintf(stderr, "decompressed file size = %lu bytes\n", outfile_stat.st_size);
        double saving = 100 * (1 - ((double)infile_stat.st_size / (double)outfile_stat.st_size));
        fprintf(stderr, "space saving = %.2f%%\n", saving);
    }

    delete_tree(&root);
    close(infile);
    close(outfile);
}
