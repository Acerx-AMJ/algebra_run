#ifndef GAME_HPP
#define GAME_HPP

// Includes

#include "states.hpp"

// Game

class Game {
   sf::RenderWindow* window;
   States states;

public:
   Game();
   ~Game();

   void init();
   void run();
};

#endif
