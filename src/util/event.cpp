#include "util/event.hpp"

// Includes

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

// Constructors

Event::Event(sf::RenderWindow& window)
   : window(window) {}

// Update functions

void Event::update() {
   dt = dt_clock.restart().asSeconds();

   keys_released.reset();
   keys_pressed.reset();
   mouse_released.reset();
   mouse_pressed.reset();

   sf::Event event;
   while (window.pollEvent(event)) {
      // Keyboard input
      
      size_t key = event.key.code;

      if (key > sf::Keyboard::Key::KeyCount) {
         key = sf::Keyboard::Key::Unknown;
      }

      if (event.type == sf::Event::KeyPressed and not keys_down.test(key) and window.hasFocus()) {
         keys_pressed.set(key);
         keys_down.set(key);
      }

      if (event.type == sf::Event::KeyReleased) {
         if (window.hasFocus()) {
            keys_released.set(key);
         }
         keys_down.reset(key);
      }

      // Mouse input

      size_t mouse = event.key.code;

      if (event.type == sf::Event::MouseButtonPressed and not mouse_down.test(mouse) and window.hasFocus()) {
         mouse_pressed.set(mouse);
         mouse_down.set(mouse);
      }

      if (event.type == sf::Event::MouseButtonReleased) {
         if (window.hasFocus()) {
            mouse_released.set(mouse);
         }
         mouse_down.reset(mouse);
      }

      if (event.type == sf::Event::Closed) {
         window.close();
      }
   }
}

// Get functions

float Event::get_dt() const {
   return dt;
}

sf::Vector2f Event::get_mouse_pos() const {
   return sf::Vector2f(sf::Mouse::getPosition(window));
}

sf::Vector2f Event::get_center() const {
   return sf::Vector2f(window.getSize() / 2u);
}

sf::Vector2f Event::get_size() const {
   return sf::Vector2f(window.getSize());
}

bool Event::is_pressed(sf::Keyboard::Key key) const {
   return keys_pressed.test(key);
}

bool Event::is_pressed(sf::Mouse::Button button) const {
   return mouse_pressed.test(button);
}

bool Event::is_released(sf::Keyboard::Key key) const {
   return keys_released.test(key);
}

bool Event::is_released(sf::Mouse::Button button) const {
   return mouse_released.test(button);
}

bool Event::is_down(sf::Keyboard::Key key) const {
   return keys_down.test(key);
}

bool Event::is_down(sf::Mouse::Button button) const {
   return mouse_down.test(button);
}
