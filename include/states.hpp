#ifndef A_HPP
#define A_HPP

// Includes

#include <deque>
#include <memory>

namespace sf {
   class RenderWindow;
}

class State;
using States = std::deque<std::unique_ptr<State>>;

// State

class State {
protected:
   sf::RenderWindow& window;
   bool quit = false;
   
public:
   State(sf::RenderWindow& window)
      : window(window) {}
   virtual ~State() = default;

   virtual void update() = 0;
   virtual void render() = 0;
   virtual void change(States& states) = 0;

   bool should_quit() const {
      return quit;
   }
};

#endif
