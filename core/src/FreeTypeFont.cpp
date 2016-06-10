/**
 * @class FreeTypeFont
 * @brief Allows to load and render to a bitmap a true type font
 *        It also implements a cache of pre-renderered letters for
 *        later use. The cache is empty upon construction
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "FreeTypeFont.hpp"

using namespace std;

#define FREETYPE_FONT_NUM_GLYPHS 128
#define FREETYPE_FONT_DPI 96

FreeTypeFont::FreeTypeFont()
{
    _ftGlyphCache.resize(FREETYPE_FONT_NUM_GLYPHS, (FT_BitmapGlyph)NULL);
    _advanceCache.resize(FREETYPE_FONT_NUM_GLYPHS, 0);
}

FreeTypeFont::~FreeTypeFont()
{
    FT_Done_Face(_ftFace);
    FT_Done_Library(_ftLibrary);
}

bool FreeTypeFont::init(const string &font_path, uint32_t size)
{
    if (FT_Init_FreeType(&_ftLibrary) != 0) {
        return false;
    }

    if (FT_New_Face(_ftLibrary, font_path.c_str(), 0, &_ftFace) != 0) {
        return false;
    }

    /* As explained in NeHe tutorial we need to multiply by 64
     * to achieve 1 unit pixel height as height is specified
     * as 1/64th of a pixel */
    FT_Set_Char_Size(_ftFace, size << 6, size << 6, FREETYPE_FONT_DPI, FREETYPE_FONT_DPI);

    _size = size;

    return true;
}

const uint8_t *FreeTypeFont::getBitmap(char letter, uint32_t &width, uint32_t &height, uint32_t &offsetLeft, uint32_t &offsetTop,
                                       uint32_t &advance)
{
    FT_Glyph glyph;

    if ((size_t)letter > _ftGlyphCache.size()) {
        return NULL;
    }

    bool check = false;
    if (_ftGlyphCache[letter] == (FT_BitmapGlyph)NULL) {
        /* Load the glyph into the cache */
        if (FT_Load_Glyph(_ftFace, FT_Get_Char_Index(_ftFace, letter), FT_LOAD_DEFAULT) != 0) {
            return NULL;
        }

        if (FT_Get_Glyph(_ftFace->glyph, &glyph) != 0) {
            return NULL;
        }

        _advanceCache[letter] = _ftFace->glyph->advance.x / 64;

        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        _ftGlyphCache[letter] = (FT_BitmapGlyph)glyph;
    }

    width = _ftGlyphCache[letter]->bitmap.width;
    height = _ftGlyphCache[letter]->bitmap.rows;
    offsetLeft = _ftGlyphCache[letter]->left;
    offsetTop = _size - _ftGlyphCache[letter]->top;
    advance = _advanceCache[letter];
    return _ftGlyphCache[letter]->bitmap.buffer;
}
