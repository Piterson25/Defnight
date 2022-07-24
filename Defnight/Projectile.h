#pragma once
#include "Text.h"

class Projectile
{
public:
	sf::Sprite sprite;
	sf::Texture texture;
	std::string name;

	float scale;
	sf::Vector2f predkosc;

	int hp;
	int attack;
	unsigned speed;
	bool collided;
	bool collidedPlayer;
	bool collidedMob;
	float angle;


	Projectile();
	virtual ~Projectile();

	void init(float& scale, const std::string& name, const int& attack, const unsigned& speed, const int& hp);
	void calculateVelocity(const sf::Vector2f& coords, const float& dt);

	const bool sideWall(const sf::Vector2f& predkosci, const sf::FloatRect& projectileBounds, const sf::FloatRect& wallBounds, const short& side);
	void checkCollision(const std::vector<sf::Sprite>& obstacles);
	void collisionPlayer(const sf::Sprite& player);
	void collisionMob(const sf::Sprite& mob);

	void move(const float& dt);
};
