#ifndef MUSICENGINE_H
#define MUSICENGINE_H

#include "header.h"

class MusicEngine
{
public:
	MusicEngine(const float& volume);
	virtual ~MusicEngine();

	const bool hasStopped() const;
	const bool isEmpty() const;

	void addMusic(const std::string& name);
	void clearMusic();
	void playMusic();
	void pauseMusic();
	void stopMusic();

	void update();
private:
	sf::Music music;
	float volume;
	std::vector<std::string> tracks;
};

#endif