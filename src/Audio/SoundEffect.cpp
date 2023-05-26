#include "SoundEffect.hpp"

SoundEffect::SoundEffect(const std::string &name, float volume) : name(name)
{
    if (!this->buffer.loadFromFile("assets/sounds/" + name + ".wav")) {
        throw("ERROR - COULDN'T LOAD SOUND:" + name);
    }
    else {
        this->sound.setBuffer(buffer);
    }
    this->sound.setVolume(volume);
    this->sound.play();
}

SoundEffect::~SoundEffect() = default;

const bool SoundEffect::hasStopped()
{
    return (this->sound.getStatus() == sf::Sound::Stopped);
}

void SoundEffect::play()
{
    this->sound.play();
}

void SoundEffect::stop()
{
    this->sound.stop();
}
