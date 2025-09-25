#ifndef UTIL_TEXT_HPP
#define UTIL_TEXT_HPP

// Includes

#include <string>

namespace sf {
   class Text;
}

// Text

namespace text {
   // Center functions

   void center(sf::Text& text);
   void set(sf::Text& text, const std::string& string);

   // Wrap functions

   void wrap(sf::Text& text, float max_width);
}

#endif
