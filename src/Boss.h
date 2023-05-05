#ifndef BOSS_H
#define BOSS_H

#include "Monster.h"
#include "TileMap.h"
#include "AIComponent.h"
#include "ProjectileSystem.h"
#include "SoundEngine.h"

class TileMap;
class AIComponent;
class ProjectileSystem;
class SoundEngine;

class Boss
	: public Monster
{
public:
	Boss(const sf::VideoMode& vm, const std::string& monster_name, sf::Texture& texture, sf::Texture& shadow_texture, TileMap* tileMap,
		const float& x, const float& y, const float& difficulty_mod, const float& wave_mod, const uint32_t& monsterSize);
	virtual ~Boss();

	void specialAttack(const float& dt, ProjectileSystem* projectileSystem, SoundEngine* soundEngine);
	void update(const float& dt, ProjectileSystem* projectileSystem, SoundEngine* soundEngine);
	void draw(sf::RenderTarget& target);
	void drawShadow(sf::RenderTarget& target);
private:
	uint32_t monsterSize;
	float specialAttackCountdown;

	AIComponent* AI;
};

#endif