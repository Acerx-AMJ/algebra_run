#ifndef UTIL_AUDIO_HPP
#define UTIL_AUDIO_HPP

// Includes

#include <SFML/Audio/Sound.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Asset;

// Audio manager

class Audio {
   struct SavedSound {
      std::vector<std::string> identifiers;
      int duplicates = 255;
      float pitch_min = 1;
      float pitch_max = 1;
   };

   Asset& asset;
   std::vector<std::unique_ptr<sf::Sound>> active_sounds;
   std::unordered_map<std::string, SavedSound> saved_sounds;
   float volume = 100;
public:

   Audio(Asset& asset);
   ~Audio() = default;

   // Update functions

   void update();

   // Sound functions

   void save(const std::string& identifier, const std::string& song_identifier, int duplicates, float pitch_min, float pitch_max);
   void save(const std::string& identifier, const std::vector<std::string>& song_identifiers, int duplicates, float pitch_min, float pitch_max);
   void play(const std::string& identifier);

   void set_volume(float volume);
   bool contains(const std::string& identifier);
};

#endif
