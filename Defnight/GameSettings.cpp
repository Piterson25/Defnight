#include "GameSettings.h"

GameSettings::GameSettings()
{
	this->path = "external/config/game_settings.ini";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->fpsLimit = 60;
	this->fpsCounterOn = true;
	this->musicVolume = 50;
	this->soundsVolume = 50;
	this->language = "english";
	this->videoModes = sf::VideoMode::getFullscreenModes();
}

void GameSettings::save()
{
	std::ofstream ofs(this->path);

	if (ofs.is_open()) {
		ofs << this->resolution.width << " " << this->resolution.height << '\n';
		ofs << this->fullscreen << '\n';
		ofs << this->fpsLimit << '\n';
		ofs << this->fpsCounterOn << '\n';
		ofs << this->musicVolume << '\n';
		ofs << this->soundsVolume << '\n';
		ofs << this->language;
	}
	ofs.close();
}

void GameSettings::load()
{
	std::ifstream ifs(this->path);

	if (ifs.is_open()) {
		ifs >> this->resolution.width >> this->resolution.height;
		ifs >> this->fullscreen;
		ifs >> this->fpsLimit;
		ifs >> this->fpsCounterOn;
		ifs >> this->musicVolume;
		ifs >> this->soundsVolume;
		ifs >> this->language;
	}
	ifs.close();
}
