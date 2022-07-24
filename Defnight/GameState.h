#pragma once
#include "Player.h"

class GameState
{
private:
	std::vector<sf::Keyboard::Key> klawisze;
	float scale;

	sf::Font font;

	sf::View view;

	sf::Texture background_texture;
	sf::Sprite background;

	sf::Texture tiles_texture;
	std::vector<sf::Sprite> obstacles;

	Monster monster;
	sf::Texture goblin_texture;
	sf::Texture spider_texture;
	sf::Texture orc_texture;
	sf::Texture cyclope_texture;
	std::list<Monster> potwory;

	sf::Texture stone_texture;
	sf::Texture shuriken_texture;
	std::list<Projectile> pociski;

	Text damage_t;
	std::list<Text> damage_texts;
	
	


	Player* player;

	sf::Texture shadow_texture;

	sf::View viewHUD;
	sf::Texture HUD_texture;
	sf::Sprite HUD;
	sf::Texture side_bar_texture;
	sf::Sprite side_bar;
	sf::Texture upgrade_bar_texture;
	sf::Sprite upgrade_bar;
	
	sf::Sprite photo;
	sf::Text playerName;

	sf::Text monsterCount;
	
	sf::Text attack;
	sf::Text attackSpeed;
	sf::Text speed;
	sf::Text critical;
	sf::Text gold;
	sf::Text waveNumber;
	unsigned wave;
	sf::Text nextWave;

	sf::Texture bars;

	sf::Text Reg;

	sf::Texture drop_texture;
	std::list<Drop> drops;


	int temp_hp_bar;
	int temp_reg_bar;
	bool regen;
	sf::Sprite hp_bar;
	sf::Text hp;

	int temp_lvl_bar;
	bool levelup;
	sf::Text levelUp_text;
	sf::Texture icons_texture;
	std::vector<sf::Sprite> icons;

	bool leveling;
	bool upgrading;
	sf::Sprite option1;
	sf::Sprite option2;
	sf::Texture option_texture;
	sf::Sprite option1_button;
	sf::Sprite option2_button;
	short option1_id;
	short option2_id;
	unsigned option1_up;
	unsigned option2_up;
	sf::Text option1_text;
	sf::Text option2_text;

	sf::Texture upgrade_texture;
	sf::Texture upgrade_icons;
	sf::Sprite upgrade1_button;
	sf::Sprite upgrade2_button;
	sf::Sprite upgrade3_button;
	sf::Sprite upgrade1_icon;
	sf::Sprite upgrade2_icon;
	sf::Sprite upgrade3_icon;
	sf::Sprite upgrade1_miniIcon;
	sf::Sprite upgrade2_miniIcon;
	sf::Sprite upgrade3_miniIcon;
	sf::Text upgrade1_text;
	sf::Text upgrade2_text;
	sf::Text upgrade3_text;

	sf::Texture ability_icon_texture;
	sf::Sprite ability_icon;
	sf::RectangleShape ability_icon_shadow;
	float ability_icon_cooldown;

	sf::Texture ability_texture;
	sf::Sprite ability;

	sf::Text level;
	sf::Sprite xp_bar;
	sf::Text xp;

	float waveCountdown;
	int numberOfMonsters;

	float titleCountdown;
	sf::Text bigWave;
	sf::Text killMobs;

	float difficultyModifier;
	unsigned sila;
	std::vector<short> idiki;

	sf::RectangleShape escScreen;
	sf::RectangleShape dieScreen;
	sf::Text dieText;
	sf::Text menu;
	sf::Text quit;
	
	bool mycha;
	float escCounter;
	bool paused;
	bool escPause;

	void initPointers();

public:
	GameState();
	virtual ~GameState();

	void init(float& scale, std::vector<sf::Keyboard::Key>& klawisze, const std::string& mapName, const std::string& playerName, const std::string& difficulty);

	void monsterType();
	void addMonsters();
	
	void draw(sf::RenderWindow& window);
	void update(sf::RenderWindow& window, const float& dt, unsigned& state);
	void gameUpdate(sf::RenderWindow& window, const float& dt, unsigned& state);
	void deathScreenUpdate(sf::RenderWindow& window, unsigned& state);

	//void button(sf::RenderWindow& window, sf::RectangleShape& rec, const std::string& word);
	void buttonSprite(sf::RenderWindow& window, sf::Sprite& spr, const std::string& word);
	void buttonText(sf::RenderWindow& window, sf::Text& text, const std::string& word, unsigned& state);
	
};
