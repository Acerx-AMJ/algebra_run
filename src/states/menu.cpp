#include "states/menu.hpp"

// Includes

#include <SFML/Graphics/RenderWindow.hpp>
#include "util/asset.hpp"
#include "util/audio.hpp"
#include "util/event.hpp"
#include "util/text.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
   static constexpr float fade_in_length = .3f;
   static constexpr float fade_out_length = .3f;

   static sf::Color colors[] { sf::Color(220, 30, 30), sf::Color(30, 30, 220), sf::Color(30, 220, 30), sf::Color(30, 130, 220), sf::Color(130, 30, 220) };
   static constexpr size_t color_count = 5;
}

// Constructors

MainMenuState::MainMenuState(sf::RenderWindow& window, Asset& asset, Audio& audio, Event& event)
   : State(window, asset, audio, event) {
   quit_button.setSize(sf::Vector2f(50.f, 50.f));
   quit_button.setOrigin(quit_button.getSize() / 2.f);
   quit_button.setPosition(40.f, 40.f);
   quit_button.setTexture(&asset.get_sprite("back"s));
   quit_button.setFillColor(sf::Color(200, 200, 200));

   title_text.setFont(asset.get_font("Monoid"s));
   title_text.setCharacterSize(96);
   title_text.setStyle(sf::Text::Style::Bold);
   title_text.setOutlineThickness(7.5f);
   title_text.setOutlineColor(sf::Color(32, 32, 32));
   title_text.setPosition(event.get_center() - sf::Vector2f(0, 300));
   text::set(title_text, "ALGEBRA RUN"s);

   auto& color = colors[rand() % color_count];
   auto& background_texture = asset.get_sprite("background1"s);
   auto& foreground_texture = asset.get_sprite("floor1"s);
   background_texture.setRepeated(true);
   foreground_texture.setRepeated(true);

   background.setSize(sf::Vector2f(event.get_size().x * 2.f, event.get_size().y));
   background.setTexture(&background_texture);
   background.setTextureRect(sf::IntRect(0, 0, background_texture.getSize().x * 2.f, background_texture.getSize().y));
   background.setFillColor(color);

   foreground.setSize(sf::Vector2f(event.get_size().x * 2.f, 175.f));
   foreground.setPosition(0.f, event.get_size().y - foreground.getSize().y);
   foreground.setTexture(&asset.get_sprite("floor1"s));
   foreground.setTextureRect(sf::IntRect(0, 0, foreground_texture.getSize().x * 2.f, foreground_texture.getSize().y));
   foreground.setFillColor(color);

   screen_tint.setSize(event.get_size());
   screen_tint.setFillColor(sf::Color(0, 0, 0, 255));
}

// Default functions

void MainMenuState::update() {
   switch (phase) {
   case Phase::idle:       update_idle();       break;
   case Phase::fading_in:  update_fading_in();  break;
   case Phase::fading_out: update_fading_out(); break;
   case Phase::quit:                            break;
   }
}

void MainMenuState::render() {
   window.draw(background);
   window.draw(foreground);

   window.draw(quit_button);
   window.draw(title_text);
   window.draw(screen_tint);
}

void MainMenuState::change(States& states) {

}

// Update functions

void MainMenuState::update_fading_in() {
   fade_in_timer += event.get_dt();
   screen_tint.setFillColor(sf::Color(0, 0, 0, 255 - (fade_in_timer / fade_in_length) * 255));

   if (fade_in_timer >= fade_out_length) {
      screen_tint.setFillColor(sf::Color(0, 0, 0, 0));
      phase = Phase::idle;
   }
}

void MainMenuState::update_fading_out() {
   fade_out_timer += event.get_dt();
   screen_tint.setFillColor(sf::Color(0, 0, 0, (fade_out_timer / fade_out_length) * 255));

   if (fade_out_timer >= fade_out_length) {
      screen_tint.setFillColor(sf::Color(0, 0, 0, 255));
      quit = true;
      phase = original;
   }
}

void MainMenuState::update_idle() {
   auto mouse = event.get_mouse_pos();
   auto dt = event.get_dt();

   if (quit_button.getGlobalBounds().contains(mouse)) {
      quit_button.setScale(1.1f, 1.1f);
      quit_button.setFillColor(sf::Color(255, 255, 255));

      if (event.is_released(sf::Mouse::Button::Left)) {
         audio.play("hit"s);
         original = phase;
         phase = Phase::fading_out;
      }
   } else {
      quit_button.setScale(1.f, 1.f);
      quit_button.setFillColor(sf::Color(200, 200, 200));
   }

   auto bg_pos = background.getPosition(), fg_pos = foreground.getPosition();
   background.setPosition(int((bg_pos.x - 150.f * dt)) % int(event.get_size().x), bg_pos.y);
   foreground.setPosition(int((fg_pos.x - 200.f * dt)) % int(event.get_size().x), fg_pos.y);
}
