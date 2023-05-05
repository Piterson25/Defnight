#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Player.hpp"
#include "Monster.hpp"
#include "Entity.hpp"
#include "FloatingTextSystem.hpp"
#include "TileMap.hpp"

class Player;
class Monster;
class FloatingTextSystem;
class TileMap;

class Projectile
	: public Entity
{
public:
	Projectile(const sf::VideoMode& vm, const std::string& projectile_name, sf::Texture& texture, const float& x, const float& y, 
		const uint32_t& attack, const uint32_t& HP, const uint32_t& speed, const sf::Vector2f& coords, const float& coordsOffset, const bool& monsterProjectile);
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

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
private:
	float timeExisting;
	bool collided;
	bool collidedPlayer;
	bool collidedMonster;
	float angle;

	bool monsterProjectile;
	bool bouncing;
};

#endif