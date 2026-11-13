#pragma once

#include "Utils/header.h"

enum class SoundType { GAME, PLAYER, MONSTER };

class SoundEffect {
public:
    SoundEffect(const std::string &name, float volume, SoundType soundType);
    ~SoundEffect();

    const bool hasStopped();

    void play();
    void stop();

private:
    std::string name;
    SoundType soundType;
    sf::SoundBuffer buffer;
    sf::Sound sound;
};
