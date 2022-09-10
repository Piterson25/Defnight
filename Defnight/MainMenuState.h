#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include "SettingsState.h"
#include "Gui.h"

class MainMenuState :
	public State
{
public:
	MainMenuState(const float& gridSize, sf::RenderWindow* window, GraphicsSettings* grap,
		std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, std::stack<State*>* states);
	virtual ~MainMenuState();

	void initGUI();
	void resetGUI();

	void update(const float& dt);
	void draw(sf::RenderTarget* target = NULL);
private:

	std::unordered_map<std::string, gui::ButtonText*> text_buttons;
	std::unordered_map<std::string, gui::Text*> texts;
	std::unordered_map<std::string, gui::ButtonSprite*> sprite_buttons;
	std::unordered_map<std::string, gui::Sprite*> sprites;

	uint16_t page;

	sf::RectangleShape dimBackground;
	float introCooldown;
	float dimAlpha;
	bool quitwindow;

	bool choosing_hero;
	sf::Texture attributes_texture;
	std::vector<sf::Sprite> attribute_vec;

	sf::Texture abilities_texture;
	std::vector<sf::Sprite> abilities_vec;

	std::string map_name;
	std::string hero_name;
	std::string difficulty_name;
};

#endif