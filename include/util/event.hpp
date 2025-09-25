#ifndef UTIL_EVENT_HPP
#define UTIL_EVENT_HPP

// Includes

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <bitset>

namespace sf {
   class RenderWindow;
}

// Event handler

class Event {
   sf::RenderWindow& window;

   sf::Clock dt_clock;
   float dt = 0;

   std::bitset<sf::Keyboard::Key::KeyCount + 1> keys_pressed, keys_released, keys_down;
   std::bitset<sf::Mouse::Button::ButtonCount + 1> mouse_pressed, mouse_released, mouse_down;

public:
   Event(sf::RenderWindow& window);
   ~Event() = default;

   // Update functions

   void update();

   // Get functions

   float get_dt() const;

   sf::Vector2f get_mouse_pos() const;
   sf::Vector2f get_center() const;
   sf::Vector2f get_size() const;

   bool is_pressed(sf::Keyboard::Key key) const;
   bool is_pressed(sf::Mouse::Button button) const;

   bool is_released(sf::Keyboard::Key key) const;
   bool is_released(sf::Mouse::Button button) const;

   bool is_down(sf::Keyboard::Key key) const;
   bool is_down(sf::Mouse::Button button) const;
};

#endif
