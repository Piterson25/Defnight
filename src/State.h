#ifndef STATE_H
#define STATE_H

#include "GameSettings.h"
#include "SoundEngine.h"
#include "MusicEngine.h"

class State
{
public:
	State(const float& gridSize, sf::RenderWindow* window, GameSettings* grap,
		std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, SoundEngine* soundEngine, MusicEngine* musicEngine, std::stack<State*>* states);
	virtual ~State();

	const bool& getReseted() const;
	const bool& getQuit() const;
	const bool getKeytime();
	const bool& getMouseClick() const;
	bool& getKeysClick1(const std::string& keyName);
	bool& getKeysClick2(const std::string& keyName);

	void endState();
	void pauseState();
	void unpauseState();
	void setMouseClick(const bool& click);
	void setKeysClick(const std::string& keyName, const bool& click);

	virtual void updateMousePositions(sf::View* view = NULL);
	virtual void updateMouseClick();
	virtual void updateKeysClick(const std::string& keyName, sf::Keyboard::Key key);
	virtual void updateKeytime(const float& dt);
	virtual void update(const float& dt) = 0;
	virtual void draw(sf::RenderTarget* target = NULL) = 0;
protected:
	float gridSize;
	sf::RenderWindow* window;
	GameSettings* gameSettings;
	std::unordered_map<std::string, int>* supportedKeys;
	sf::Font font;
	SoundEngine* soundEngine;
	MusicEngine* musicEngine;
	std::stack<State*>* states;
	
	std::unordered_map<std::string, int> keybinds;
	std::unordered_map<std::string, std::string> lang;
	std::unordered_map<std::string, std::pair<bool, bool>> keysClick;
	
	float keytime;
	float keytimeMax;
	bool quit;
	bool paused;
	bool mouseClick;
	bool reseted;

	std::unordered_map<std::string, sf::Texture> textures;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
};

#endif