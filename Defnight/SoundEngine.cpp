#include "stdafx.h"
#include "SoundEngine.h"

SoundEngine::SoundEngine(const float& volume)
	:volume(volume)
{
	
}

SoundEngine::~SoundEngine()
{
	this->sounds.clear();
}

void SoundEngine::addSound(const std::string& name)
{
	this->sounds.emplace_back(new SoundEffect(name, this->volume));
}

void SoundEngine::playSounds()
{
	for (const auto& s : this->sounds) {
		if (s->hasStopped()) s->play();
	}
}

void SoundEngine::stopSounds()
{
	for (const auto& s : this->sounds) {
		if (!s->hasStopped()) s->stop();
	}
}

void SoundEngine::update()
{
	for (auto s = this->sounds.begin(); s != this->sounds.end();) {
		if ((*s)->hasStopped()) s = this->sounds.erase(s);
		else ++s;
	}
}
