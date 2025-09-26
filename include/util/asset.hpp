#ifndef UTIL_ASSET_HPP
#define UTIL_ASSET_HPP

// Includes

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <functional>
#include <mutex>

// Asset manager

class Asset {
   using Sprite = sf::Texture;
   using Sound = sf::SoundBuffer;
   using Font = sf::Font;
   using Callback = std::function<void()>;

   std::filesystem::path root;   
   std::unordered_map<std::string, Sprite> sprites;
   std::unordered_map<std::string, Sound> sounds;
   std::unordered_map<std::string, Font> fonts;

   std::mutex sprite_mutex;
   std::mutex sound_mutex;
   std::mutex font_mutex;
public:

   Asset(const std::filesystem::path& root);
   ~Asset() = default;

   // Loading functions

   Sprite& load_sprite(const std::filesystem::path& path);
   Sound& load_sound(const std::filesystem::path& path);
   Font& load_font(const std::filesystem::path& path);

   // Directory loading functions

   void load_sprite_directory(const Callback& callback, const std::filesystem::path& path);
   void load_sound_directory(const Callback& callback, const std::filesystem::path& path);
   void load_font_directory(const Callback& callback, const std::filesystem::path& path);

   // Get functions

   Sprite& get_sprite(const std::string& identifier);
   Sound& get_sound(const std::string& identifier);
   Font& get_font(const std::string& identifier);

   // Contains functions

   bool contains_sprite(const std::string& identifier);
   bool contains_sound(const std::string& identifier);
   bool contains_font(const std::string& identifier);
};

#endif
