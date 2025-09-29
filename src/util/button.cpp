#include "util/button.hpp"

// Includes

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "util/audio.hpp"
#include "util/event.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
   constexpr float hold_delay = .4f;
   constexpr float hold_inter = .1f;
}

// Button object

void Button::init(Audio* audio) {
   this->audio = audio;
   rect.setFillColor(sf::Color(200, 200, 200));
   text.setFillColor(sf::Color(200, 200, 200));
}

void Button::place(const sf::Vector2f& pos) {
   rect.setPosition(pos);
   text.setPosition(rect.getPosition() + rect.getSize() / 2.f);
   text.setOrigin(text.getGlobalBounds().getSize() / 2.f);
}

void Button::update(bool selected, bool down, bool released) {
   clicked = selected and released;
   contains = selected;
   
   if (not selected) {
      rect.setScale(1.f, 1.f);
      text.setScale(1.f, 1.f);
      rect.setFillColor(sf::Color(200, 200, 200));
      text.setFillColor(sf::Color(200, 200, 200));
      return;
   }
   rect.setScale(1.1f, 1.1f);
   text.setScale(1.1f, 1.1f);
   rect.setFillColor(sf::Color(255, 255, 255));
   text.setFillColor(sf::Color(255, 255, 255));

   if (released) {
      audio->play("hit"s);
   }
   if (down) {
      rect.setScale(.9f, .9f);
      text.setScale(.9f, .9f);
   }
}

void Button::update(Event& event) {
   bool selected = rect.getGlobalBounds().contains(event.get_mouse_pos());
   update(selected, selected and event.is_down(sf::Mouse::Button::Left), selected and event.is_released(sf::Mouse::Button::Left));
}

// Update/render functions

void Navigation::update(Event& event, Audio& audio) {
   auto previous = index;

   bool up_down = event.is_down(sf::Keyboard::Key::Up);
   bool down_down = event.is_down(sf::Keyboard::Key::Down);

   bool should_go_up = event.is_released(sf::Keyboard::Key::Up);
   bool should_go_down = event.is_released(sf::Keyboard::Key::Down);

   if (up_down or down_down) {
      if (not holding) {
         hold_timer.restart();
         holding = true;
      } else if (hold_timer.getElapsedTime().asSeconds() >= hold_delay and interval_timer.getElapsedTime().asSeconds() >= hold_inter) {
         interval_timer.restart();
         should_go_up = should_go_up or up_down;
         should_go_down = should_go_down or down_down;
      }
   } else {
      holding = false;
   }

   if (should_go_up) {
      index = (index <= first ? last : index - 1);
   }

   if (should_go_down) {
      index = (index >= last ? first : index + 1);
   }

   auto mouse_pos = event.get_mouse_pos();
   auto mouse_released = event.is_released(sf::Mouse::Button::Left);
   auto mouse_down = event.is_down(sf::Mouse::Button::Left);
   changed = previous != index;

   for (auto& [eindex, element] : elements) {
      bool selected = element->rect.getGlobalBounds().contains(mouse_pos);
      bool released = selected and mouse_released;
      bool down = selected and mouse_down;

      if (eindex == index) {
         selected = true;
         down = event.is_down(sf::Keyboard::Key::Enter);
         released = event.is_released(sf::Keyboard::Key::Enter);
      }
      element->update(selected, down, released);
   }
}

void Navigation::render(sf::RenderWindow& window) const {
   for (const auto& [_, element] : elements) {
      window.draw(element->rect);
      window.draw(element->text);
   }
}

// Set functions

void Navigation::set_index(size_t index) {
   this->index = index;
}

void Navigation::reset_index() {
   this->index = 0;
}

// Get functions

Navigation::Element& Navigation::get_element(size_t index) {
   return *elements[index];
}

Navigation::Element& Navigation::get_element() {
   return *elements[index];
}

size_t Navigation::get_index() const {
   return index;
}

bool Navigation::any_selected() const {
   return elements.contains(index);
}

bool Navigation::selection_changed() const {
   return changed;
}

Navigation::Map& Navigation::get_elements() {
   return elements;
}
