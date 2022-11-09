#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "SoundEffect.h"

class SoundEngine
{
public:
	SoundEngine(const float& volume);
	virtual ~SoundEngine();

	void addSound(const std::string& name);
	void playSounds();
	void stopSounds();
	void deleteSound();
private:
	std::list<std::unique_ptr<SoundEffect>> sounds;
	float volume;
};

#endif