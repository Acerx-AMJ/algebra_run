#ifndef UTIL_BUTTON_HPP
#define UTIL_BUTTON_HPP

// Includes

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <unordered_map>

class Audio;
class Event;

namespace sf {
   class RenderWindow;
}

// Button object

struct Button {
   sf::RectangleShape rect;
   sf::Text text;
   Audio* audio;

   bool clicked = false, contains = false;

   void init(Audio* audio);
   void place(const sf::Vector2f& pos);
   void update(bool selected, bool down, bool released);
   void update(Event& event);
};

// Navigation manager

class Navigation {
   using Element = Button;
   using Map = std::unordered_map<size_t, Element*>;

   Map elements;
   sf::Clock hold_timer, interval_timer;

   size_t index = 0;
   size_t last = 0;
   size_t first = -1;
   bool changed = false;
   bool holding = false;

public:
   Navigation() = default;
   ~Navigation() = default;

   // Update/render functions

   void update(Event& event, Audio& audio);
   void render(sf::RenderWindow& window) const;

   // Set functions

   void set_index(size_t index);
   void reset_index();

   template<typename... Args>
   void add_elements(Args&&... args) {
      if (elements.empty()) {
         first = 1;
      }

      size_t start = last;
      (elements.insert({++start, &std::forward<Args>(args)}), ...);
      last = start;
   }

   // Get functions

   Element& get_element(size_t index);
   Element& get_element();
   size_t get_index() const;
   bool any_selected() const;
   bool selection_changed() const;
   Map& get_elements();
};

#endif
