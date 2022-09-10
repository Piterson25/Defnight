#include "Functions.h"
#include "GraphicsSettings.h"

GraphicsSettings::GraphicsSettings()
{
	this->path = "external/config/graphics.ini";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->fpsLimit = 60;
	this->videoModes = sf::VideoMode::getFullscreenModes();
	this->language = "english";
}

void GraphicsSettings::save()
{
	std::ofstream ofs(this->path);

	if (ofs.is_open()) {
		ofs << this->resolution.width << " " << this->resolution.height << '\n';
		ofs << this->fullscreen << '\n';
		ofs << this->fpsLimit << '\n';
		ofs << this->language;
	}
	ofs.close();
}

void GraphicsSettings::load()
{
	std::ifstream ifs(this->path);

	if (ifs.is_open()) {
		ifs >> this->resolution.width >> this->resolution.height;
		ifs >> this->fullscreen;
		ifs >> this->fpsLimit;
		ifs >> this->language;
	}
	ifs.close();
}
