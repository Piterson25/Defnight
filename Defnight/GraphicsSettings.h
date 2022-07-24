#pragma once

class GraphicsSettings
{
public:

	GraphicsSettings();

	sf::VideoMode resolution;
	bool fullscreen;
	bool verticalSync;

	std::vector<sf::VideoMode> videoModes;

	void saveToFile(const std::string path);
	void loadFromFile(const std::string path);
};
