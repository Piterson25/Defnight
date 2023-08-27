#pragma once

#include "SoundEffect.hpp"

class SoundEngine {
public:
    SoundEngine(float volume);
    ~SoundEngine();

    void addSound(const std::string &name);
    void playSounds();
    void setVolume(float t_volume);
    void stopSounds();

    void update();

private:
    std::list<std::unique_ptr<SoundEffect>> sounds;
    float volume;
};
