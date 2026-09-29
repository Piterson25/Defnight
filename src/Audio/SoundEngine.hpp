#pragma once

#include "SoundEffect.hpp"

class SoundEngine {
public:
    SoundEngine(float gameVolume, float playerVolume, float monsterVolume);
    ~SoundEngine();

    void addSound(const std::string &name);
    void playSounds();
    void setVolume(float t_volume, SoundType soundType);
    void stopSounds();

    void update();

private:
    std::unordered_map<std::string, SoundType> soundsMap;
    std::list<std::unique_ptr<SoundEffect>> sounds;
    float gameVolume;
    float playerVolume;
    float monsterVolume;
};
