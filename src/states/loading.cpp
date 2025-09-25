#include "states/loading.hpp"

// Includes

#include <nlohmann/json.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "game.hpp"
#include "util/asset.hpp"
#include "util/error.hpp"
#include "util/event.hpp"
#include "util/audio.hpp"
#include "util/text.hpp"
#include <fstream>
#include <thread>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
   static constexpr float done_length = 1.f;
   static constexpr float fade_length = .3f;
   static constexpr size_t phase_count = 4;
   static constexpr float phase_progress = 1.f / phase_count;

   static constexpr float start_min = .25f;
   static constexpr float start_max = .75f;
   static constexpr const char* fallback_splash = "Something isn't quite right!";
}

// Constructors

LoadingState::LoadingState(sf::RenderWindow& window, Asset& asset, Audio& audio, Event& event, Game& game)
   : game(game), State(window, asset, audio, event) {
   const auto& bar_texture = asset.load_sprite("images/ui/loading_bar.png"s);
   const auto& font = asset.load_font("fonts/Monoid.ttf"s);

   loading_bar_bg.setSize(sf::Vector2f(600.f, 40.f));
   loading_bar_bg.setOrigin(loading_bar_bg.getSize() / 2.f);
   loading_bar_bg.setPosition(event.get_center());
   loading_bar_bg.setTexture(&bar_texture);
   loading_bar_bg.setFillColor(sf::Color(125, 125, 125));

   loading_bar_fg.setSize(loading_bar_bg.getSize());
   loading_bar_fg.setOrigin(loading_bar_bg.getOrigin());
   loading_bar_fg.setPosition(loading_bar_bg.getPosition());
   loading_bar_fg.setTexture(&bar_texture);
   loading_bar_fg.setFillColor(sf::Color(255, 255, 255));

   err::assert(loading_bar_shader.loadFromFile("assets/shaders/clip.frag"s, sf::Shader::Fragment), "Could not load shader 'clip.frag'."s);
   loading_bar_shader.setUniform("texture"s, sf::Shader::CurrentTexture);
   loading_bar_shader.setUniform("progress"s, 0);

   loading_text.setFont(font);
   loading_text.setCharacterSize(48);
   loading_text.setPosition(event.get_center() - sf::Vector2f(0, 75));
   text::set(loading_text, "Loading..."s);

   info_text.setFont(font);
   info_text.setCharacterSize(24);
   info_text.setPosition(event.get_center() + sf::Vector2f(0, 75));
   text::set(info_text, get_splash());
   text::wrap(info_text, window.getSize().x - 120.f);

   screen_tint.setSize(event.get_size());
   screen_tint.setFillColor(sf::Color(0, 0, 0, 0));

   start_length = start_min + rand() / (RAND_MAX / (start_max - start_min));
}

// Default functions

void LoadingState::update() {
   switch (phase) {
   case Phase::fading_in:  update_fading_in();  break;
   case Phase::sprites:    load_sprites();      break;
   case Phase::sounds:     load_sounds();       break;
   case Phase::fonts:      load_fonts();        break;
   case Phase::data:       setup_game_data();   break;
   case Phase::fading_out: update_fading_out(); break;
   case Phase::waiting:                         break;
   }
}

void LoadingState::render() {
   window.draw(loading_bar_bg);
   window.draw(loading_bar_fg, &loading_bar_shader);
   window.draw(loading_text);
   window.draw(info_text);
   window.draw(screen_tint);
}

void LoadingState::change(States& states) {
   // Push back the main menu state
}

// Update functions

void LoadingState::update_fading_in() {
   start_timer += event.get_dt();
   if (start_timer >= start_length) {
      phase = Phase::sprites;
   }
}

void LoadingState::load_sprites() {
   text::set(loading_text, "Loading Sprites..."s);
   phase = Phase::waiting;

   asset.load_sprite_directory([this](){
      phase = Phase::sounds;
      progress += phase_progress;
      loading_bar_shader.setUniform("progress"s, progress);
   }, "images/"s);
}

void LoadingState::load_sounds() {
   text::set(loading_text, "Loading Sounds..."s);
   phase = Phase::waiting;

   asset.load_sound_directory([this](){
      phase = Phase::fonts;
      progress += phase_progress;
      loading_bar_shader.setUniform("progress"s, progress);
   }, "sounds/"s);
}

void LoadingState::load_fonts() {
   text::set(loading_text, "Loading Fonts..."s);
   phase = Phase::waiting;

   asset.load_font_directory([this](){
      phase = Phase::data;
      progress += phase_progress;
      loading_bar_shader.setUniform("progress"s, progress);
   }, "fonts/"s);
}

void LoadingState::setup_game_data() {
   text::set(loading_text, "Loading Game Data..."s);
   phase = Phase::waiting;

   std::thread([this](){
      game.init();
      text::set(loading_text, "Loading Done!"s);
      audio.play("success"s);
      phase = Phase::fading_out;
      loading_bar_shader.setUniform("progress"s, 1.f);
   }).detach();
}

void LoadingState::update_fading_out() {
   done_timer += event.get_dt();

   if (done_timer >= done_length) {
      fade_timer += event.get_dt();
      screen_tint.setFillColor(sf::Color(0, 0, 0, (fade_timer / fade_length) * 255));
   }

   if (fade_timer >= fade_length) {
      screen_tint.setFillColor(sf::Color(0, 0, 0, 255));
      quit = true;
   }
}

// Private functions

std::string LoadingState::get_splash() const {
   std::ifstream file ("data/splash.json"s);
   if (!file.is_open()) {
      return fallback_splash;
   }

   nlohmann::json data;
   file >> data;
   file.close();

   if (not data.contains("splash"s) or not data["splash"s].is_array()) {
      return fallback_splash;
   }

   auto splash = data["splash"s][rand() % data["splash"s].size()];
   return (splash.is_string() ? splash : fallback_splash);
}
