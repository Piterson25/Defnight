#include "SoundEffect.h"

SoundEffect::SoundEffect(const std::string& name, const float& volume)
	:name(name)
{
    if (!this->buffer.loadFromFile("external/music/" + name + ".wav")) {
        throw("ERROR - COULDN'T LOAD SOUND:" + name);
    }
    else {
        this->sound.setBuffer(buffer);
    }
	this->sound.setVolume(volume);
	this->sound.stop();
}

SoundEffect::~SoundEffect()
{

}

const bool SoundEffect::hasStopped()
{
    if (this->sound.getStatus() == sf::Sound::Stopped) return true;
    return false;
}

void SoundEffect::play()
{
	this->sound.play();
}

void SoundEffect::stop()
{
	this->sound.stop();
}
