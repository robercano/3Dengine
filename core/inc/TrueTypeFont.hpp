/**
 * @class TrueTypeFont
 * @brief Allows to load and render to a bitmap a true type font
 *        It also implements a cache of pre-renderered letters for
 *        later use. The cache is empty upon construction
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include <stdint.h>

class TrueTypeFont
{
    public:
        static TrueTypeFont *New();
        static void Delete(TrueTypeFont *font);

        virtual ~TrueTypeFont() {};
        virtual bool init(const std::string &font_path, uint32_t size) = 0;
        virtual const uint8_t *getBitmap(char letter, uint32_t &width, uint32_t &height,
                                         uint32_t &offsetLeft, uint32_t &offsetTop, uint32_t &advance) = 0;
};
