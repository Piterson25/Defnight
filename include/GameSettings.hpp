#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include "header.h"

class GameSettings
{
public:
	GameSettings();

	std::string path;
	sf::VideoMode resolution;
	bool fullscreen;
	uint32_t fpsLimit;
	bool fpsCounterOn;
	float musicVolume;
	float soundsVolume;
	std::string language;

	std::vector<sf::VideoMode> videoModes;

	void save();
	void load();
};

#endif