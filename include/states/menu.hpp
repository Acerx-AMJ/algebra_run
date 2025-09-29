#ifndef STATES_MENU_HPP
#define STATES_MENU_HPP

// Includes

#include "states.hpp"
#include "util/button.hpp"

// Main menu state

class MainMenuState : public State {
   enum class Phase { idle, quit, fading_in, fading_out };

   Button quit_button;
   Button play_button;
   sf::Text title_text;

   sf::RectangleShape background;
   sf::RectangleShape foreground;
   sf::RectangleShape screen_tint;

   float fade_in_timer = 0;
   float fade_out_timer = 0;
   float dt = 0;
   Phase phase = Phase::fading_in, original = phase;
public:

   MainMenuState(sf::RenderWindow& window, Asset& asset, Audio& audio, Event& event);
   ~MainMenuState() = default;

   // Default functions

   void update() override;
   void render() override;
   void change(States& states) override;

   // Update functions

   void update_fading_in();
   void update_fading_out();
   void update_idle();
   void move_background();
};

#endif
