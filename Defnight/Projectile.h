#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Player.h"
#include "Monster.h"
#include "Entity.h"
#include "FloatingTextSystem.h"
#include "TileMap.h"

class Player;
class Monster;
class FloatingTextSystem;
class TileMap;

class Projectile
	: public Entity
{
public:
	Projectile(const sf::VideoMode& vm, const std::string& projectile_name, sf::Texture& texture, const float& x, const float& y, 
		const uint32_t& attack, const uint32_t& HP, const uint32_t& speed, const sf::Vector2f& coords, const float& coordsOffset);
	virtual ~Projectile();

	virtual const bool getCollided() const;
	virtual const bool getCollidedPlayer() const;
	virtual const bool getCollidedMonster() const;
	virtual const float getTimeExisting() const;
	virtual const bool getExploded() const;
	virtual const bool isBomb() const;

	void calculateVelocity(const sf::Vector2f& coords);
	
	void wallCollision(TileMap* tileMap);
	void playerCollision(Player* player);
	void monsterCollision(Monster* monster, Player* player, FloatingTextSystem* floatingTextSystem);

	void updateVelocity(const float& dt);

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
private:
	float timeExisting;
	bool collided;
	bool collidedPlayer;
	bool collidedMonster;
	float angle;

	bool bouncing;
};

#endif