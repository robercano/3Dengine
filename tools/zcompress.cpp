#include <stdio.h>
#include <fstream>
#include "ZCompression.hpp"

#define CHUNK_SIZE 2133

int main(int argc, char **argv)
{
    uint8_t buffer[CHUNK_SIZE];
    uint32_t ret = -1;

    if (argc < 4) {
        fprintf(stderr, "Tool to compress and decompress files\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "    zcompress <operation> <input> <output>\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "operation - 'c' for compress and 'd' for decompress, without quotes\n");
        fprintf(stderr, "input     - Input file\n");
        fprintf(stderr, "output    - Output file\n");
        fprintf(stderr, "\n");

        exit(1);
    }

    switch (argv[1][0]) {
        case 'c':
        {
            ZCompression comp;
            FILE *input = fopen(argv[2], "rb");
            std::ofstream output(argv[3], std::ios::binary | std::ios::trunc | std::ios::out);

            if (input == NULL) {
                fprintf(stderr, "ERROR opening input file %s\n", argv[2]);
                goto error_comp;
            }

            if (output.is_open() == false) {
                fprintf(stderr, "ERROR opening output file %s\n", argv[2]);
                goto error_comp;
            }

            if (comp.init() == false) {
                fprintf(stderr, "ERROR initializing compressor\n");
                goto error_comp;
            }

            while (feof(input) == 0) {
                size_t bytesRead = fread(buffer, 1, CHUNK_SIZE, input);

                if (comp.write(output, buffer, (uint32_t)bytesRead, feof(input) != 0) == false) {
                    fprintf(stderr, "ERROR writing compressed data\n");
                    goto error_comp;
                }
            }

            /* Everything OK */
            ret = 0;

error_comp:
            comp.finish();
            fclose(input);
            output.close();
        }
        break;
        case 'd':
        {
            ZDecompression dcomp;
            uint32_t bytesRead;
            std::ifstream input(argv[2], std::ios::binary | std::ios::in);
            FILE *output= fopen(argv[3], "wb");

            if (input.is_open() == false) {
                fprintf(stderr, "ERROR opening input file %s\n", argv[2]);
                goto error_dec;
            }

            if (output == NULL) {
                fprintf(stderr, "ERROR opening output file %s\n", argv[2]);
                goto error_dec;
            }

            if (dcomp.init() == false) {
                fprintf(stderr, "ERROR initializing decompressor\n");
                goto error_dec;
            }

            do {
                bytesRead = CHUNK_SIZE;

                if (dcomp.read(input, buffer, bytesRead) == false) {
                    fprintf(stderr, "ERROR writing decompressed data\n");
                    goto error_dec;
                }

                if (fwrite(buffer, 1, bytesRead, output) != bytesRead) {
                    fprintf(stderr, "ERROR writing to file %s\n", argv[3]);
                    goto error_dec;
                }
            } while (bytesRead != 0);

            /* Everything OK */
            ret = 0;

error_dec:
            dcomp.finish();
            input.close();
            fclose(output);
        }
        break;
        default:
            fprintf(stderr, "ERROR wrong operation option, use 'c' or 'd' without quotes\n");
    }

    if (ret == 0) {
        fprintf(stderr, "Operation finished succesfully\n");
    }
    exit(ret);
}
