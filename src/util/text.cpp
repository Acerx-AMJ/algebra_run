#include "util/text.hpp"

// Includes

#include <SFML/Graphics/Text.hpp>
#include <sstream>

using namespace std::string_literals;
using namespace std::string_view_literals;

// Text

namespace text {
   // Center functions

   void center(sf::Text& text) {
      text.setOrigin(text.getGlobalBounds().getSize() / 2.f);
   }

   void set(sf::Text& text, const std::string& string) {
      text.setString(string);
      text::center(text);
   }

   // Wrap functions

   void wrap(sf::Text& text, float max_width) {
      if (text.getGlobalBounds().width <= max_width or text.getString().isEmpty()) {
         return;
      }

      std::string original = text.getString();
      std::string_view view = original, split = original;
      std::stringstream result;

      while (text.getGlobalBounds().width > max_width) {
         size_t left = 0, right = split.size();
         std::string_view truncated;

         while (left < right) {
            size_t mid = (left + right) / 2;
            truncated = split.substr(0, mid);

            text.setString(std::string(truncated) + "-"s);
            (text.getGlobalBounds().width > max_width ? (right = mid) : (left = mid + 1));
         }
         truncated = split.substr(0, left - 1);
         split = split.substr(left - 1);

         bool stop = isalpha(truncated.back()) and isalpha(split.front());
         result << truncated << (stop ? "-\n" : "\n");

         text.setString(std::string(split));
         if (text.getGlobalBounds().width <= max_width) {
            result << split;
            break;
         }
      }
      text.setString(result.str());
      text::center(text);
   }
}
