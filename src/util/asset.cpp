#include "util/asset.hpp"

// Includes

#include <thread>
#include "util/error.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

// Constructors

Asset::Asset(const std::filesystem::path& root)
   : root(root) {
   err::assert(std::filesystem::exists(root), "Directory '"s + root.string() + "' does not exist."s);
   err::assert(std::filesystem::is_directory(root), "Directory '"s + root.string() + "' is not a valid directory."s);
}

// Loading functions

Asset::Sprite& Asset::load_sprite(const std::filesystem::path& path) {
   auto full (root / path);
   err::assert(std::filesystem::exists(full), "File '"s + full.string() + "' does not exist."s);

   auto identifier (path.stem().string());
   if (sprites.contains(identifier)) {
      return sprites[identifier];
   }

   Sprite sprite;
   err::assert(sprite.loadFromFile(full), "Could not load texture from file '"s + full.string() + "'."s);

   sprites.insert({identifier, sprite});
   return sprites[identifier];
}

Asset::Sound& Asset::load_sound(const std::filesystem::path& path) {
   auto full (root / path);
   err::assert(std::filesystem::exists(full), "File '"s + full.string() + "' does not exist."s);

   auto identifier (path.stem().string());
   if (sounds.contains(identifier)) {
      return sounds[identifier];
   }

   Sound sound;
   err::assert(sound.loadFromFile(full), "Could not load sound from file '"s + full.string() + "'."s);

   sounds.insert({identifier, sound});
   return sounds[identifier];
}

Asset::Font& Asset::load_font(const std::filesystem::path& path) {
   auto full (root / path);
   err::assert(std::filesystem::exists(full), "File '"s + full.string() + "' does not exist."s);

   auto identifier (path.stem().string());
   if (fonts.contains(identifier)) {
      return fonts[identifier];
   }

   Font font;
   err::assert(font.loadFromFile(full), "Could not load font from file '"s + full.string() + "'."s);

   fonts.insert({identifier, font});
   return fonts[identifier];
}

// Directory loading functions

void Asset::load_sprite_directory(const Callback& callback, const std::filesystem::path& path) {
   auto full (root / path);
   err::assert(std::filesystem::exists(full), "Directory '"s + full.string() + "' does not exist."s);
   err::assert(std::filesystem::is_directory(full), "Directory '"s + full.string() + "' is not a valid directory."s);

   std::thread([=, this]() {
      std::function<void(const std::filesystem::path&)> load = [&](const std::filesystem::path& path) {
         for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (file.is_directory()) {
               load(file.path());
            }

            if (not file.is_regular_file()) {
               continue;
            }

            Sprite sprite;
            if (not sprite.loadFromFile(file.path())) {
               continue;
            }

            std::lock_guard<std::mutex> lock(sprite_mutex);
            sprites.insert({file.path().stem(), sprite});
         }
      };

      load(full);
      if (callback) {
         callback();
      }
   }).detach();
}

void Asset::load_sound_directory(const Callback& callback, const std::filesystem::path& path) {
   auto full (root / path);
   err::assert(std::filesystem::exists(full), "Directory '"s + full.string() + "' does not exist."s);
   err::assert(std::filesystem::is_directory(full), "Directory '"s + full.string() + "' is not a valid directory."s);

   std::thread([=, this]() {
      std::function<void(const std::filesystem::path&)> load = [&](const std::filesystem::path& path) {
         for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (file.is_directory()) {
               load(file.path());
            }

            if (not file.is_regular_file()) {
               continue;
            }

            Sound sound;
            if (not sound.loadFromFile(file.path())) {
               continue;
            }

            std::lock_guard<std::mutex> lock(sound_mutex);
            sounds.insert({file.path().stem(), sound});
         }
      };

      load(full);
      if (callback) {
         callback();
      }
   }).detach();
}

void Asset::load_font_directory(const Callback& callback, const std::filesystem::path& path) {
   auto full (root / path);
   err::assert(std::filesystem::exists(full), "Directory '"s + full.string() + "' does not exist."s);
   err::assert(std::filesystem::is_directory(full), "Directory '"s + full.string() + "' is not a valid directory."s);

   std::thread([=, this]() {
      std::function<void(const std::filesystem::path&)> load = [&](const std::filesystem::path& path) {
         for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (file.is_directory()) {
               load(file.path());
            }

            if (not file.is_regular_file()) {
               continue;
            }

            Font font;
            if (not font.loadFromFile(file.path())) {
               continue;
            }

            std::lock_guard<std::mutex> lock(font_mutex);
            fonts.insert({file.path().stem(), font});
         }
      };

      load(full);
      if (callback) {
         callback();
      }
   }).detach();
}

// Get functions

Asset::Sprite& Asset::get_sprite(const std::string& identifier) {
   err::assert(sprites.contains(identifier), "Texture '"s + identifier + "' does not exist."s);
   return sprites[identifier];
}

Asset::Sound& Asset::get_sound(const std::string& identifier) {
   err::assert(sounds.contains(identifier), "Sound '"s + identifier + "' does not exist."s);
   return sounds[identifier];
}

Asset::Font& Asset::get_font(const std::string& identifier) {
   err::assert(fonts.contains(identifier), "Font '"s + identifier + "' does not exist."s);
   return fonts[identifier];
}

// Contains functions

bool Asset::contains_sprite(const std::string& identifier) {
   return sprites.contains(identifier);
}

bool Asset::contains_sound(const std::string& identifier) {
   return sounds.contains(identifier);
}

bool Asset::contains_font(const std::string& identifier) {
   return fonts.contains(identifier);
}
