#include "util/audio.hpp"

// Includes

#include <SFML/Audio/SoundBuffer.hpp>
#include "util/asset.hpp"
#include "util/error.hpp"
#include <algorithm>

using namespace std::string_literals;
using namespace std::string_view_literals;

// Constructors

Audio::Audio(Asset& asset)
   : asset(asset) {}

// Update functions

void Audio::update() {
   active_sounds.erase(std::remove_if(active_sounds.begin(), active_sounds.end(), [](const auto& sound) {
      return sound->getStatus() == sf::Sound::Stopped;
   }), active_sounds.end());
}

// Sound functions

void Audio::save(const std::string& identifier, const std::string& song_identifier, int duplicates, float pitch_min, float pitch_max) {
   saved_sounds[identifier] = SavedSound{{song_identifier}, duplicates, pitch_min, pitch_max};
}

void Audio::save(const std::string& identifier, const std::vector<std::string>& song_identifiers, int duplicates, float pitch_min, float pitch_max) {
   saved_sounds[identifier] = SavedSound{song_identifiers, duplicates, pitch_min, pitch_max};
}

void Audio::play(const std::string& identifier) {
   err::assert(saved_sounds.contains(identifier), "Sound '"s + identifier + "' has not been saved."s);
   auto& sound = saved_sounds[identifier];

   if (sound.identifiers.empty()) {
      return;
   }

   auto& sound_identifier = sound.identifiers[rand() % sound.identifiers.size()];
   err::assert(asset.contains_sound(sound_identifier), "Sound '"s + sound_identifier + "' does not exist."s);

   int total = 0;
   for (const auto& sound : active_sounds) {
      total += (sound->getBuffer() == &asset.get_sound(sound_identifier));
   }

   if (sound.duplicates <= total) {
      return;
   }

   auto new_sound = std::make_unique<sf::Sound>();
   new_sound->setBuffer(asset.get_sound(sound_identifier));
   new_sound->setVolume(volume);
   new_sound->setPitch(sound.pitch_min + rand() / (RAND_MAX / (sound.pitch_max - sound.pitch_min)));
   new_sound->play();
   active_sounds.push_back(std::move(new_sound));
}

void Audio::set_volume(float volume) {
   this->volume = volume;
   for (auto& sound : active_sounds) {
      sound->setVolume(volume);
   }
}

bool Audio::contains(const std::string& identifier) {
   return saved_sounds.contains(identifier);
}
