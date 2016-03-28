
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "PNGLoader.h"

int loadPNG(const char *filename, uint8_t **image, uint32_t *width, uint32_t *height, uint32_t *bytesPerPixel)
{
	int x, y;
	int ret = 0;
	//png_byte color_type;
	png_byte bit_depth;
	png_structp png_ptr;
	png_infop info_ptr;
	//int number_of_passes;
	png_bytep * row_pointers;
	uint32_t row_size_bytes;

	char header[8];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "ERROR opening PNG file %s\n", filename);
		return -1;
	}

	if (fread(header, sizeof *header, sizeof header, fp) != sizeof header) {
		fprintf(stderr, "ERROR reading PNG header from %s\n", filename);
		return -1;
	}

	if (png_sig_cmp(header, 0, sizeof header)) {
		fprintf(stderr, "ERROR file %s is not recognized as a PNG file", filename);
		return -1;
	}

	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fprintf(stderr, "ERROR png_create_read_struct failed for %s\n", filename);
		return -1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "ERROR png_create_info_struct failed for %s\n", filename);
		return -1;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "ERROR during init_io for %s\n", filename);
		return -1;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	*width = png_get_image_width(png_ptr, info_ptr);
	*height = png_get_image_height(png_ptr, info_ptr);
	//color_type = png_get_color_type(png_ptr, info_ptr);
	*bytesPerPixel = png_get_bit_depth(png_ptr, info_ptr) / 8;

	//number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	/* read file */
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "ERROR during read_image for %s\n", filename);
		return -1;
	}

	row_size_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*image = malloc(row_size_bytes * (*height));
	if ((*image) == NULL) {
		fprintf(stderr, "ERROR allocating memory for final image for %s\n", filename);
		return -1;
	}

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * (*height));
	if (row_pointers == NULL) {
		fprintf(stderr, "ERROR allocating memory for final image for %s\n", filename);
		free(*image);
		*image = NULL;
		return -1;
	}

	memset(row_pointers, 0, sizeof(png_bytep) * (*height));

	for (y=0; y<(*height); y++) {
		row_pointers[y] = (png_byte*) malloc(row_size_bytes);
		if (row_pointers[y] == NULL) {
			fprintf(stderr, "ERROR allocating memory for the row number %d for %s\n", y, filename);
			ret = -1;
			goto error;
		}
	}

	png_read_image(png_ptr, row_pointers);

	fclose(fp);

	/* Now copy the data to the final location */
	for (y=0; y<(*height); y++) {
		memcpy(&(*image)[row_size_bytes * y], row_pointers[y], row_size_bytes);
	}

error:
	for (y=0; y<(*height); y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);
	return ret;
}
