#ifndef GAME_H
#define GAME_H

#include "MainMenuState.h"

class Game
{
public:
	Game();
	virtual ~Game();

	void init();

	void checkEvents();
	void close();
	void draw();
	void update();
	void run();
private:
	sf::RenderWindow* window;
	GraphicsSettings graphicsSettings;
	sf::Event sfEvent;
	std::unordered_map<std::string, int> supportedKeys;

	sf::Clock dtClock;
	float dt;

	sf::Font font;
	gui::Text* fpsCounter;
	uint16_t fps;
	float fpsTimer;

	std::stack<State*> states;

	float gridSize;
};

#endif