#include "game.hpp"

// Includes

#include <SFML/Graphics/RenderWindow.hpp>
#include "states/loading.hpp"
#include "util/asset.hpp"
#include "util/audio.hpp"
#include "util/event.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
   static constexpr size_t framerate_limit = 60;
   static constexpr bool vsync_enabled = true;
}

// Constructors

Game::Game() {
   srand(time(nullptr));
   
   window = new sf::RenderWindow(sf::VideoMode().getDesktopMode(), "Algebra Run"s, sf::Style::Fullscreen);
   window->setVerticalSyncEnabled(vsync_enabled);
   window->setFramerateLimit(framerate_limit);

   asset = new Asset("assets"s);
   audio = new Audio(*asset);
   event = new Event(*window);
   states.push_back(std::make_unique<LoadingState>(*window, *asset, *audio, *event, *this));
}

Game::~Game() {
   window->close();
   delete event;
   delete asset;
   delete audio;
   delete window;
}

// Run function

void Game::run() {
   while (window->isOpen()) {
      if (states.front()->should_quit()) {
         states.front()->change(states);
         states.pop_front();
      }

      if (states.empty()) {
         return;
      }

      event->update();
      audio->update();
      states.front()->update();

      window->clear();
      states.front()->render();
      window->display();
   }
}

// Initialize functions

void Game::init() {
   init_audio();
}

void Game::init_audio() {
   audio->save("success"s, "success"s, 255, .75f, 1.25f);
}
