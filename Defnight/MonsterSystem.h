#ifndef MONSTERSYSTEM_H
#define MONSTERSYSTEM_H

#include "Monster.h"
#include "Player.h"
#include "PlayerGUI.h"
#include "TileMap.h"
#include "SoundEngine.h"
#include "DropSystem.h"
#include "Particle.h"
#include "ProjectileSystem.h"
#include "Projectile.h"
#include "FloatingTextSystem.h"

class Monster;
class Player;
class PlayerGUI;
class TileMap;
class SoundEngine;
class DropSystem;
class Particle;
class ProjectileSystem;
class Projectile;
class FloatingTextSystem;

class MonsterSystem
{
public:
	MonsterSystem(const sf::VideoMode& vm, TileMap* tileMap, const float& gridSize, const float& difficulty_mod);
	virtual ~MonsterSystem();

	virtual const bool monsterIDsEmpty() const;
	virtual const size_t monsterIDsSize() const;
	virtual const bool monstersEmpty() const;
	virtual const bool isBossWave() const;
	virtual const std::vector<sf::Vector2f> monstersPositions() const;
	const float bossHP() const;

	void monsterIDsClear();

	void addMonster(const std::string& monster_name, sf::Texture& texture, 
		const float& x, const float& y, const float& wave_mod);

	void playerAttack(Player* player, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine, bool& playedSound);
	void explosionAttack(Particle* particle, FloatingTextSystem* floatingTextSystem);

	void projectileCollision(Projectile* proj, Player* player, FloatingTextSystem* floatingTextSystem);

	void monsterCollision(Monster* mob);
	void spawnMonsters(Player* player, const uint32_t& wave);
	void prepareWave(uint32_t& wave, uint32_t& sumHP);

	void update(Player* player, PlayerGUI* playerGUI, ProjectileSystem* projectileSystem, DropSystem* dropSystem, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine, bool& paused, const float& dt);
	void draw(sf::RenderTarget& target);
	void drawShadow(sf::RenderTarget& target);
private:
	sf::VideoMode vm;
	std::list<std::unique_ptr<Monster>> monsters;
	std::unordered_map<std::string, sf::Texture> textures;
	sf::Texture shadow_texture;
	TileMap* tileMap;
	std::vector<short> monsterIDs;
	float gridSize;
	float difficulty_mod;
	bool bossWave;
};

#endif