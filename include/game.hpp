#ifndef GAME_HPP
#define GAME_HPP

// Includes

#include "states.hpp"

// Game

class Game {
   sf::RenderWindow* window;
   Asset* asset;
   Audio* audio;
   Event* event;
   States states;

   void init_audio();

public:
   Game();
   ~Game();

   void init();
   void run();
};

#endif
