#include "game.hpp"

// Includes
#include <SFML/Graphics/RenderWindow.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
   static constexpr size_t framerate_limit = 60;
   static constexpr bool vsync_enabled = true;
}

Game::Game() {
   srand(time(nullptr));
   
   window = new sf::RenderWindow(sf::VideoMode().getDesktopMode(), "Algebra Run"s, sf::Style::Fullscreen);
   window->setVerticalSyncEnabled(vsync_enabled);
   window->setFramerateLimit(framerate_limit);

   // Push back a state in the future
}

Game::~Game() {
   window->close();
   delete window;
}

void Game::init() {
   // For now, do not initialize anything
}

void Game::run() {
   while (window->isOpen()) {
      if (states.front()->should_quit()) {
         states.front()->change(states);
         states.pop_front();
      }

      if (states.empty()) {
         return;
      }

      states.front()->update();

      window->clear();
      states.front()->render();
      window->display();
   }
}
