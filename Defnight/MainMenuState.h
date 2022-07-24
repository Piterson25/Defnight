#pragma once
#include "GameState.h"
#include "GraphicsSettings.h"

class MainMenuState
{
private:
	std::vector<sf::Keyboard::Key> klawisze;
	float scale;

	unsigned state;

	sf::Text fpsCounter;
	float fps;
	sf::Clock fpsClock;

	sf::Font font;

	sf::Texture title_texture;
	sf::Sprite title;

	sf::RectangleShape dimBackground;
	sf::Text quitText;
	bool quitwindow;
	sf::Text yes;
	sf::Text no;

	sf::Text play;
	
	sf::Text quit;
	sf::Text version;

	sf::Text closequit;
	bool applybool;
	sf::Text settings;

	sf::Text fullhd;
	sf::Text hd;
	bool isFullHD;

	sf::Text yesFullscreen;
	sf::Text noFullscreen;
	bool isFullscreen;

	sf::Text yesVsync;
	sf::Text noVsync;
	bool isVsync;

	sf::Text wsad;
	sf::Text strzalki;
	bool isWsad;

	sf::Text fpsOn;
	sf::Text fpsOff;
	bool isFps;

	sf::Text apply;

	sf::Text test;
	sf::Text settings_text;

	sf::Text chooseMap_text;

	sf::Texture background_texture;
	sf::Texture select_map_texture;
	sf::Sprite select_map;
	sf::Sprite mapa1;
	sf::Text mapName;

	sf::Text chooseClass_text;

	sf::Texture player;
	sf::Texture select_hero_texture;
	sf::Sprite select_hero;
	sf::Sprite wojownik;
	sf::Text className;


	sf::Text chooseDifficulty_text;
	sf::Texture select_difficulty_texture;
	sf::Sprite select_easy;
	sf::Sprite select_normal;
	sf::Sprite select_hard;
	sf::Texture difficulty_texture;
	sf::Sprite easy;
	sf::Sprite normal;
	sf::Sprite hard;
	sf::Text easy_text;
	sf::Text normal_text;
	sf::Text hard_text;
	sf::Text easy_desc;
	sf::Text normal_desc;
	sf::Text hard_desc;
	

	sf::Texture go_back_texture;
	sf::Sprite go_back;

	GameState gra;

	bool mycha;

public:
	MainMenuState();
	virtual ~MainMenuState();

	void init(float& scale, std::vector<sf::Keyboard::Key>& klawisze, const bool& isFullhd, const bool& isFullscreen, const bool& isVsync, const bool& isWsad, const bool& isFps);

	void update(sf::RenderWindow& window, const float& dt);
	void draw(sf::RenderWindow& window);

	void buttonText(sf::RenderWindow& window, sf::Text& t, const std::string& s);
	void buttonTextChoice(sf::RenderWindow& window, sf::Text& text, const std::string& word);
	void buttonSprite(sf::RenderWindow& window, sf::Sprite& spr, const std::string& word);

	void state0(sf::RenderWindow& window);
	void state1(sf::RenderWindow& window);
	void state2(sf::RenderWindow& window);
	void state3(sf::RenderWindow& window);
	void state5(sf::RenderWindow& window);
};
