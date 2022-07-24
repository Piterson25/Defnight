#pragma once
#include "MainMenuState.h"

class Game
{
private:
	GraphicsSettings graphicsSettings;
	sf::RenderWindow window;
	sf::Event sfEvent;
	std::vector<sf::Keyboard::Key> klawisze;
	float scale;

	sf::Clock dtClock;
	float dt;

	float gridSize;

	MainMenuState menu;
public:
	Game();
	virtual ~Game();
	
	void check_events();
	void close();
	void draw();
	void update();
	void run();
};
