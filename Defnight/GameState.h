#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "Monster.h"
#include "State.h"
#include "Gui.h"
#include "PlayerGUI.h"
#include "FloatingText.h"
#include "Drop.h"
#include "Tile.h"
#include "SettingsState.h"

class GameState :
	public State
{
public:
	GameState(const float& gridSize, sf::RenderWindow* window, GameSettings* grap,
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

	sf::Music music;
	std::vector<std::string> tracks;
	size_t currentTrackNumber;

	std::unordered_map<std::string, gui::ButtonText*> text_buttons;
	std::unordered_map<std::string, gui::Text*> texts;
	std::unordered_map<std::string, gui::ButtonSprite*> sprite_buttons;
	std::unordered_map<std::string, gui::Sprite*> sprites;

	sf::Sprite background;
	sf::Texture background_texture;

	sf::Texture tiles_texture;
	sf::VertexArray vertexArray;
	std::vector<Tile*> tiles;

	Player* player;
	PlayerGUI* playerGUI;

	float difficultyModifier;
	std::list<Monster*> monsters;
	std::vector<short> monsterIDs;

	uint16_t wave;
	uint16_t sumHP;
	float waveCountdown;

	std::list<FloatingText*> floatingTexts;

	sf::SoundBuffer coinBuffer;
	sf::Sound coinSound;
	std::list<Drop*> drops;

	std::list<Projectile*> projectiles;
};

#endif