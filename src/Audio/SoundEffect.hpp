#pragma once

#include "Utils/header.h"

class SoundEffect {
public:
    SoundEffect(const std::string &name, float volume);
    ~SoundEffect();

    const bool hasStopped();

    void play();
    void stop();

private:
    std::string name;
    sf::SoundBuffer buffer;
    sf::Sound sound;
};
