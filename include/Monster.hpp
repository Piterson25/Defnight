#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Player.hpp"
#include "Entity.hpp"
#include "ProjectileSystem.hpp"
#include "FloatingTextSystem.hpp"
#include "AIComponent.hpp"
#include "SoundEngine.hpp"
#include "TileMap.hpp"

class Player;
class ProjectileSystem;
class FloatingTextSystem;
class AIComponent;
class SoundEngine;
class TileMap;

class Monster
	: public Entity
{
public:
	Monster(const sf::VideoMode& vm, const std::string& monster_name, sf::Texture& texture, sf::Texture& shadow_texture, TileMap* tileMap, const float& x, const float& y, const float& difficulty_mod, const float& wave_mod);
	virtual ~Monster();

	virtual const uint32_t getGold() const;
	virtual const bool getSpawned() const;
	virtual const bool getDeadCountdown() const;

	const bool attackPlayer(Player* player, TileMap* tileMap, ProjectileSystem* projectileSystem, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine);
	const bool sightCollision(TileMap* tileMap, const sf::Vector2f& a_p1, const sf::Vector2f& a_p2);
	void spawn(const float& dt);
	const bool dying(const float& dt);

	void setGold(const uint32_t& gold);

	void calculateAI(TileMap* tileMap, Player* player, const std::vector<sf::Vector2f>& positions, const float& dt);

	virtual void update(const float& dt, ProjectileSystem* projectileSystem, SoundEngine* soundEngine);
	virtual void draw(sf::RenderTarget& target);
	virtual void drawShadow(sf::RenderTarget& target);
protected:
	sf::Sprite shadow;
	sf::Texture shadow_texture;

	uint32_t gold;
	uint32_t monsterSize;

	bool spawned;
	float deadCountdown;
	float spawnCountdown;

	AIComponent* AI;

	bool playedSound;
};
#endif