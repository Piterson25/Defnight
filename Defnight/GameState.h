#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "Monster.h"
#include "State.h"
#include "Gui.h"
#include "PlayerGUI.h"
#include "FloatingText.h"
#include "Drop.h"

class GameState :
	public State
{
public:
	GameState(const float& gridSize, sf::RenderWindow* window, GraphicsSettings* grap,
		std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, std::stack<State*>* states,
		const std::string& map_name, const std::string& hero_name, const std::string& difficulty_name);
	virtual ~GameState();

	void initGUI();
	void resetGUI();

	void prepareWave();
	void spawnMonsters();

	void update(const float& dt);
	void draw(sf::RenderTarget* target = NULL);
private:
	sf::View view;
	sf::View viewHUD;

	std::unordered_map<std::string, gui::ButtonText*> text_buttons;
	std::unordered_map<std::string, gui::Text*> texts;
	std::unordered_map<std::string, gui::ButtonSprite*> sprite_buttons;
	std::unordered_map<std::string, gui::Sprite*> sprites;

	sf::Sprite background;
	sf::Texture background_texture;

	sf::Texture tiles_texture;
	std::vector<sf::Sprite> obstacles;

	Player* player;
	PlayerGUI* playerGUI;

	float difficultyModifier;
	std::list<Monster*> monsters;
	std::vector<short> monsterIDs;

	unsigned wave;
	unsigned sumHP;
	float waveCountdown;

	std::list<FloatingText*> floatingTexts;

	std::list<Drop*> drops;

	std::list<Projectile*> projectiles;
};

#endif