#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Player.h"
#include "Monster.h"
#include "FloatingTextSystem.h"
#include "TileMap.h"

class Player;
class Monster;
class FloatingTextSystem;
class TileMap;

class Projectile
{
public:
	Projectile(const std::string& name, const float& posX, const float& posY,
		const unsigned& attack, const unsigned& speed, const unsigned& hp, const sf::Vector2f& coords, sf::VideoMode& vm);
	virtual ~Projectile();

	virtual const sf::Vector2f& getPosition() const;
	virtual const unsigned getHP() const;

	virtual const bool getCollided() const;
	virtual const bool getCollidedPlayer() const;
	virtual const bool getCollidedMonster() const;
	virtual const unsigned getAttack() const;

	void calculateVelocity(const sf::Vector2f& coords);

	const bool sideWall(const sf::Vector2f& velocities, const sf::FloatRect& projectileBounds, const sf::FloatRect& wallBounds, const short& side);
	void obstacleCollision(TileMap* tileMap);
	void playerCollision(Player* player);
	void monsterCollision(Monster* monster, sf::Font* font, Player* player, FloatingTextSystem* floatingTextSystem);

	void move(const float& dt);

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
private:
	sf::Sprite sprite;
	sf::Texture texture;
	std::string name;

	sf::Vector2f velocity;
	unsigned HP;
	unsigned attack;
	unsigned speed;
	bool collided;
	bool collidedPlayer;
	bool collidedMonster;
	float angle;

	sf::VideoMode vm;
};

#endif