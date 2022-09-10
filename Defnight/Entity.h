#ifndef ENTITY_H
#define ENTITY_H

#include "Functions.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual const sf::Vector2f& getPosition() const;
	virtual const sf::Texture& getTexture() const;
	virtual const sf::Vector2f getVelocity() const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const sf::Vector2f getCenter() const;
	virtual const std::string getName() const;
	virtual const bool isDead() const;
	virtual const bool getLeft() const;
	virtual const bool getUp() const;
	virtual const bool getRight() const;
	virtual const bool getDown() const;
	virtual const bool getIsAttacking() const;
	virtual const unsigned getAttack() const;
	virtual const unsigned getAttackSpeed() const;
	virtual const unsigned getMaxHP() const;
	virtual const unsigned getHP() const;
	virtual const unsigned getXP() const;
	virtual const unsigned getSpeed() const;
	virtual const unsigned getReach() const;
	virtual const unsigned getGold() const;
	virtual const unsigned getPunched() const;
	virtual const unsigned getFrame() const;


	void setVeloctiy(const sf::Vector2f& velocity);
	void setTexturePath(const std::string& path);
	void setTexture(const sf::Texture& texture);
	void setPosition(const float& x, const float& y);
	void setAttack(const unsigned& attack);
	void setAttackSpeed(const unsigned& attackSpeed);
	void setMaxHP(const unsigned& maxHP);
	void setHP(const unsigned& HP);
	void setXP(const unsigned& XP);
	void setSpeed(const unsigned& speed);
	void setReach(const unsigned& reach);
	void setGold(const unsigned& gold);
	void setName(const std::string& name);

	void loadAttack(const float& dt);
	void doAttack();
	void obstacleCollision(const std::vector<sf::Sprite>& obstacles);
	void animation(const float& dt);
	void punch();
	void smashed(const float& dt);
	void move();

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
protected:
	sf::Sprite sprite;
	sf::Texture texture;
	std::string name;

	sf::Vector2f velocity;

	unsigned attack;
	unsigned attackSpeed;
	unsigned maxHP;
	unsigned HP;
	unsigned XP;
	unsigned speed;
	unsigned reach;
	unsigned gold;

	bool up;
	bool left;
	bool down;
	bool right;

	float animationCooldown;
	unsigned frame;

	float attackCooldown;
	bool isAttacking;
	float punchedCooldown;
	bool punched;
};

#endif