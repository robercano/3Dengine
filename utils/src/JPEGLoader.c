
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <jerror.h>
#include "JPEGLoader.h"

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

    jpeg_mem_src(&cinfo, jpeg, size);
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
