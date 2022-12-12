#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "SettingsState.h"
#include "Gui.h"
#include "PlayerGUI.h"
#include "Player.h"
#include "MonsterSystem.h"
#include "ProjectileSystem.h"
#include "FloatingTextSystem.h"
#include "DropSystem.h"
#include "TileMap.h"
#include "ParticleSystem.h"

class GameState :
	public State
{
public:
	GameState(const float& gridSize, sf::RenderWindow* window, GameSettings* grap,
		std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, SoundEngine* soundEngine, MusicEngine* musicEngine, std::stack<State*>* states,
		const std::string& map_name, const std::string& hero_name, const std::string& difficulty_name);
	virtual ~GameState();

	void initGUI();
	void resetGUI();

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

	PlayerGUI* playerGUI;
	Player* player;
	MonsterSystem* monsterSystem;
	ProjectileSystem* projectileSystem;
	FloatingTextSystem* floatingTextSystem;
	DropSystem* dropSystem;
	sf::Texture tiles_texture;
	sf::VertexArray vertexArray;
	TileMap* tileMap;
	ParticleSystem* particleSystem;
	
	uint32_t wave;
	uint32_t sumHP;
	uint32_t gems;
	float waveCountdown;
};

#endif