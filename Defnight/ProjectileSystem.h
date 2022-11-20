#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H

#include "Projectile.h"
#include "Player.h"
#include "Monster.h"
#include "PlayerGUI.h"
#include "FloatingTextSystem.h"
#include "TileMap.h"

class Projectile;
class Player;
class Monster;
class PlayerGUI;
class FloatingTextSystem;
class TileMap;

class ProjectileSystem
{
public:
	ProjectileSystem(const sf::VideoMode& vm, sf::Texture& textures);
	virtual ~ProjectileSystem();

	void addProjectile(const std::string& projectile_name, const float& x, const float& y,
		const uint32_t& attack, const uint32_t& HP, const uint32_t& speed, const sf::Vector2f& coords);
	void update(Player* player, PlayerGUI* playerGui, const std::list<Monster*>& monsters, sf::Sprite& background, TileMap* tileMap, FloatingTextSystem* floatingTextSystem, const float& dt);
	void draw(sf::RenderTarget& target);
private:
	std::list<std::unique_ptr<Projectile>> projectiles;
	sf::VideoMode vm;
	sf::Texture textures;
};

#endif