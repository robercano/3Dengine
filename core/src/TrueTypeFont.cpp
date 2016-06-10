/**
 * @class TrueTypeFont
 * @brief Allows to load and render to a bitmap a true type font
 *        It also implements a cache of pre-renderered letters for
 *        later use. The cache is empty upon construction
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "TrueTypeFont.hpp"
#include "FreeTypeFont.hpp"

using namespace std;

TrueTypeFont *TrueTypeFont::New(void) { return new FreeTypeFont(); }
void TrueTypeFont::Delete(TrueTypeFont *font) { delete font; }
