#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.hpp"
#include "SettingsState.hpp"
#include "GUI.hpp"

class MainMenuState :
	public State
{
public:
	MainMenuState(const float& gridSize, sf::RenderWindow* window, GameSettings* grap,
		std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, SoundEngine* soundEngine, MusicEngine* musicEngine, std::stack<State*>* states);
	virtual ~MainMenuState();

	void initGUI();
	void resetGUI();

	void update(const float& dt);
	void draw(sf::RenderTarget* target = NULL);
private:
	void fadingEffect(const float& dt);

	std::unordered_map<std::string, gui::ButtonText*> text_buttons;
	std::unordered_map<std::string, gui::Text*> texts;
	std::unordered_map<std::string, gui::ButtonSprite*> sprite_buttons;
	std::unordered_map<std::string, gui::Sprite*> sprites;

	uint16_t page;

	sf::View mapView;
	sf::Sprite map;
	sf::Texture map_texture;
	sf::RectangleShape dimMap;
	sf::RectangleShape quitBackground;
	sf::Vector2f mapVelocity;
	float mapRotate;

	sf::RectangleShape dimBackground;
	float introCooldown;
	float dimAlpha;
	bool fading;
	bool quitwindow;
	bool appearing;

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