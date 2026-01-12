#ifndef BITMAPFONT_HPP
#define BITMAPFONT_HPP

/**
 * @file BitmapFont.hpp
 * @brief Simple bitmap font for rendering text without external libraries
 */


#include <string>
#include <cstdint>

namespace Graphitech {
namespace UI {

/**
 * @brief Simple 5x7 bitmap font
 */
class BitmapFont {
public:
    static const int FONT_CHAR_WIDTH = 5;
    static const int FONT_CHAR_HEIGHT = 7;
    
    // Get bitmap data for a character (5x7 pixels)
    // Returns nullptr if character not supported
    static const uint8_t* GetCharBitmap(char c);
    
    // Calculate text width in pixels
    static int GetTextWidth(const std::string& text, float scale = 1.0f);
    
    // Calculate text height in pixels
    static int GetTextHeight(float scale = 1.0f);
};

} // namespace UI
} // namespace Graphitech

#endif // BITMAPFONT_HPP
