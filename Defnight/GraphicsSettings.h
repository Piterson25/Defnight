#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

class GraphicsSettings
{
public:
	GraphicsSettings();

	std::string path;
	sf::VideoMode resolution;
	bool fullscreen;
	uint16_t fpsLimit;
	std::string language;

	std::vector<sf::VideoMode> videoModes;

	void save();
	void load();
};

#endif