#pragma once
#include "Monster.h"

class Player
{
public:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Sprite shadow;
	std::string name;
	float scale;

	float animationTime;
	int step;
	sf::Vector2f predkosc;

	bool punched;
	float punchedTime;
	bool attackAni;
	bool up;
	bool down;
	bool left;
	bool right;

	int attack;
	float attackCooldown;
	int attackSpeed;
	float regCooldown;
	int Reg;
	int maxhp;
	int hp;
	unsigned Level;
	int maxxp;
	int xp;
	int speed;
	int critical;
	int reach;
	unsigned gold;

	float abilityCounter;
	float abilityCooldown;
	float abilityDuration;
	bool abilityActive;

	bool dead;

	Player();
	virtual ~Player();

	void init(float& scale, const std::string& nazwa);

	void controls(std::vector<sf::Keyboard::Key>& klawisze, const float& dt);
	void animation(const float& dt);

	void collision(const std::vector<sf::Sprite>& obstacles);

	void damaged(const float& dt);

	void move();

	bool regeneration(const float& dt);

	void attackLoad(const float& dt);

	void ability(sf::RenderWindow& window, std::list<Projectile>& pociski, Projectile pocisk, const sf::Texture& texture, const sf::View& view, const float& dt);
};

