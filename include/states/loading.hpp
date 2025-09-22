#ifndef STATES_LOADING_HPP
#define STATES_LOADING_HPP

// Includes

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "states.hpp"

class Game;

// Loading state

class LoadingState : public State {
   enum class Phase { fading_in, fading_out, waiting };

   Game& game;
   sf::RectangleShape screen_tint;
   sf::RectangleShape loading_bar_fg;
   sf::RectangleShape loading_bar_bg;
   sf::Text loading_text;
   sf::Text info_text;

   float start_timer = 0;
   float start_length = 0;
   float done_timer = 0;
   float fade_timer = 0;
   Phase phase = Phase::fading_in;

   std::string get_splash() const;

public:
   LoadingState(sf::RenderWindow& window, Game& game);
   ~LoadingState() = default;

   // Default functions

   void update() override;
   void render() override;
   void change(States& states) override;

   // Update functions

   void update_fading_in();
   void update_fading_out();
};

#endif
