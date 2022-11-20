#ifndef ENTITY_H
#define ENTITY_H

#include "TileMap.h"

class TileMap;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual const sf::Vector2f& getPosition() const;
	virtual const sf::Vector2f& getVelocity() const;
	virtual const std::string& getName() const;
	virtual const sf::Texture& getTexture() const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const sf::Vector2f getCenter() const;
	virtual const sf::Vector2f getLeftCenter() const;
	virtual const sf::Vector2f getUpCenter() const;
	virtual const sf::Vector2f getDownCenter() const;
	virtual const sf::Vector2f getRightCenter() const;

	const float attackDistance(Entity* e1, Entity* e2) const;
	
	virtual const bool isDead() const;
	virtual const bool hasVelocity() const;
	virtual const bool getLeft() const;
	virtual const bool getUp() const;
	virtual const bool getRight() const;
	virtual const bool getDown() const;
	virtual const bool getIsAttacking() const;
	virtual const uint32_t getAttack() const;
	virtual const uint32_t getAttackSpeed() const;
	virtual const uint32_t getMaxHP() const;
	virtual const uint32_t getHP() const;
	virtual const uint32_t getXP() const;
	virtual const uint32_t getSpeed() const;
	virtual const uint32_t getReach() const;
	virtual const uint32_t getPunched() const;
	virtual const uint32_t getFrame() const;


	void setVeloctiy(const sf::Vector2f& velocity);
	void setTexturePath(const std::string& path);
	void setTexture(const sf::Texture& texture);
	void setPosition(const float& x, const float& y);
	void setAttack(const uint32_t& attack);
	void setAttackSpeed(const uint32_t& attackSpeed);
	void setMaxHP(const uint32_t& maxHP);
	void setHP(const uint32_t& HP);
	void setXP(const uint32_t& XP);
	void setSpeed(const uint32_t& speed);
	void setReach(const uint32_t& reach);
	void setName(const std::string& name);

	void loadAttack(const float& dt);
	void doAttack();
	void obstacleCollision(TileMap* tileMap);
	void animation(const float& dt);
	void punch();
	void smashed(const float& dt);
	void move();

protected:
	sf::VideoMode vm;
	sf::Sprite sprite;
	sf::Texture texture;
	std::string name;

	sf::Vector2f velocity;

	uint32_t attack;
	uint32_t attackSpeed;
	uint32_t maxHP;
	uint32_t HP;
	uint32_t XP;
	uint32_t speed;
	uint32_t reach;

	bool up;
	bool left;
	bool down;
	bool right;

	float animationCooldown;
	uint32_t frame;

	float attackCooldown;
	bool isAttacking;
	float punchedCooldown;
	bool punched;
};

#endif