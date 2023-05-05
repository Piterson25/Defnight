#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H

#include "Projectile.h"
#include "Player.h"
#include "MonsterSystem.h"
#include "PlayerGUI.h"
#include "FloatingTextSystem.h"
#include "TileMap.h"
#include "ParticleSystem.h"
#include "SoundEngine.h"

class Projectile;
class Player;
class MonsterSystem;
class PlayerGUI;
class FloatingTextSystem;
class TileMap;
class ParticleSystem;
class SoundEngine;

class ProjectileSystem
{
public:
	ProjectileSystem(const sf::VideoMode& vm);
	virtual ~ProjectileSystem();

	void addProjectile(const std::string& projectile_name, const float& x, const float& y,
		const uint32_t& attack, const uint32_t& HP, const uint32_t& speed, const sf::Vector2f& coords, const float& coordsOffset, const bool& monsterProjectile);
	void update(Player* player, PlayerGUI* playerGui, ParticleSystem* particleSystem, MonsterSystem* monsterSystem, sf::Sprite& background, TileMap* tileMap, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine, const float& dt);
	void draw(sf::RenderTarget& target);
private:
	std::list<std::unique_ptr<Projectile>> projectiles;
	sf::VideoMode vm;
	sf::Texture textures;
};

#endif