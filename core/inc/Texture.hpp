/**
 * @class	Texture
 * @brief	Holds the texture data
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <string.h>

class Texture
{
  public:
    Texture() : _texture(NULL), _width(0), _height(0), _Bpp(0) {}
    Texture(uint8_t *texture, uint32_t width, uint32_t height, uint32_t Bpp) : _width(width), _height(height), _Bpp(Bpp)
    {
        if (texture != NULL) {
            _texture = new uint8_t[_width * _height * _Bpp];
            memcpy(_texture, texture, _width * _height * _Bpp);
        }
    }

    uint8_t *_texture;
    uint32_t _width;
    uint32_t _height;
    uint32_t _Bpp;
};
