#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include "Functions.h"

class SoundEffect
{
public:
	SoundEffect(const std::string& name, const float& volume);
	virtual ~SoundEffect();

	virtual const bool hasStopped();

	void play();
	void stop();
private:
	std::string name;
	sf::SoundBuffer buffer;
	sf::Sound sound;
};

#endif