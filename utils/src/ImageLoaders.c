
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <jerror.h>
#include <png.h>
#include "ImageLoaders.h"

#if defined (_WIN32) || defined(_WIN64)

/* Read JPEG image from a memory segment */
static void init_source(j_decompress_ptr cinfo) {}
static boolean fill_input_buffer(j_decompress_ptr cinfo)
{
	ERREXIT(cinfo, JERR_INPUT_EMPTY);
	return TRUE;
}
static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	struct jpeg_source_mgr* src = (struct jpeg_source_mgr*) cinfo->src;

	if (num_bytes > 0) {
		src->next_input_byte += (size_t)num_bytes;
		src->bytes_in_buffer -= (size_t)num_bytes;
	}
}

static void term_source(j_decompress_ptr cinfo) {}
static void jpeg_mem_src(j_decompress_ptr cinfo, void* buffer, long nbytes)
{
	struct jpeg_source_mgr* src;

	if (cinfo->src == NULL) {   /* first time for this JPEG object? */
		cinfo->src = (struct jpeg_source_mgr *)
			(*cinfo->mem->alloc_small) ((j_common_ptr)cinfo, JPOOL_PERMANENT,
				sizeof(struct jpeg_source_mgr));
	}

	src = (struct jpeg_source_mgr*) cinfo->src;
	src->init_source = init_source;
	src->fill_input_buffer = fill_input_buffer;
	src->skip_input_data = skip_input_data;
	src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->term_source = term_source;
	src->bytes_in_buffer = nbytes;
	src->next_input_byte = (JOCTET*)buffer;
}
#endif

int loadJPEG(const char *filename, uint8_t **image, uint32_t *width, uint32_t *height, uint32_t *bytesPerPixel)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int rowStride;

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "ERROR opening JPEG file %s\n", filename);
        return -1;
    }

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (size == -1) {
		fclose(file);
		return -1;
	}
	char *jpeg = (char*)malloc(size);
	size_t count = fread(jpeg, sizeof(char), size, file);
	if (count != size) {
		free(jpeg);
		fclose(file);
		return -1;
	}

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, (unsigned char*)jpeg, size);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height = cinfo.output_height;
    *bytesPerPixel = cinfo.out_color_components;

    *image = (uint8_t*)malloc((*width) * (*height) * (*bytesPerPixel));

    rowStride = cinfo.output_width * cinfo.output_components;

    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, rowStride, 1);

    while (cinfo.output_scanline < cinfo.output_height) {
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(&(*image)[(cinfo.output_scanline-1)*rowStride], buffer[0], rowStride);
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

	free(jpeg);
    fclose(file);

    return 0;
}

int loadPNG(const char *filename, uint8_t **image, uint32_t *width, uint32_t *height, uint32_t *bytesPerPixel)
{
	uint32_t y;
	uint32_t ret = 0;
	//png_byte color_type;
	//png_byte bit_depth;
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

	if (png_sig_cmp((png_bytep)header, 0, sizeof header)) {
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
