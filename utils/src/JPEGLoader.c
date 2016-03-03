
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include "JPEGLoader.h"

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

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, file);
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

    fclose(file);

    return 0;
}
