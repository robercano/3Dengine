/**
 * @class FreeTypeFont
 * @brief Allows to load and render to a bitmap a true type font
 *        It also implements a cache of pre-renderered letters for
 *        later use. The cache is empty upon construction
 *
 * @author Roberto Cano
 */
#include "FreeTypeFont.hpp"

using namespace std;

FreeTypeFont::FreeTypeFont()
{
    _ftGlyphCache.resize(128, (FT_BitmapGlyph)NULL);
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
   FT_Set_Char_Size(_ftFace, size << 6, size << 6, 96, 96);

   return true;
}

const uint8_t *FreeTypeFont::getBitmap(char letter, uint32_t &width, uint32_t &height)
{
    FT_Glyph glyph;

    if (letter > _ftGlyphCache.size()) {
        return NULL;
    }

    if (_ftGlyphCache[letter] == (FT_BitmapGlyph)NULL) {
        /* Load the glyph into the cache */
        if (FT_Load_Glyph(_ftFace, FT_Get_Char_Index(_ftFace, letter), FT_LOAD_DEFAULT) != 0) {
            return NULL;
        }

        if (FT_Get_Glyph(_ftFace->glyph, &glyph) != 0) {
            return NULL;
        }

        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
        _ftGlyphCache[letter] = (FT_BitmapGlyph)glyph;
    }

    width = _ftGlyphCache[letter]->bitmap.width;
    height = _ftGlyphCache[letter]->bitmap.rows;
    return _ftGlyphCache[letter]->bitmap.buffer;
}
