#include "states/loading.hpp"

// Includes

#include <SFML/Graphics/RenderWindow.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
   static constexpr float done_length = 1.f;
   static constexpr float fade_length = .3f;
   static constexpr size_t phase_count = 1;
   static constexpr float phase_progress = 1.f / phase_count;

   static constexpr float start_min = .25f;
   static constexpr float start_max = .75f;
   static constexpr const char* fallback_splash = "Something isn't quite right!";
}

// Constructors

LoadingState::LoadingState(sf::RenderWindow& window, Game& game)
   : game(game), State(window) {
   
}

// Default functions

void LoadingState::update() {
   switch (phase) {
   case Phase::fading_in:  update_fading_in();  break;
   case Phase::fading_out: update_fading_out(); break;
   case Phase::waiting:                         break;
   }
}

void LoadingState::render() {
   window.draw(loading_bar_bg);
   window.draw(loading_bar_fg);
   window.draw(loading_text);
   window.draw(info_text);
   window.draw(screen_tint);
}

void LoadingState::change(States& states) {
   // Push back the main menu state
}

// Update functions

void LoadingState::update_fading_in() {
   
}

void LoadingState::update_fading_out() {

}

// Private functions

std::string LoadingState::get_splash() const {
   return fallback_splash;
}
