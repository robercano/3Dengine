/**
 * @file    ImageLoader.h
 * @brief	Several image loader helpers
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <stdio.h>   // Because jpeg.h does not include it for FILE *sighs*
#include <stdlib.h>  // Because jpeg.h does not include it for size_t *sighs*

namespace ImageLoaders
{
int loadJPEG(const char *filename, uint8_t **image, uint32_t *width, uint32_t *height, uint32_t *bytesPerPixel);
int loadPNG(const char *filename, uint8_t **image, uint32_t *width, uint32_t *height, uint32_t *bytesPerPixel);
};
