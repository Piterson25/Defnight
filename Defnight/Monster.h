#ifndef MONSTER_H
#define MONSTER_H

#include "Projectile.h"
#include "Player.h"
#include "Entity.h"
#include "FloatingText.h"

class Projectile;
class Player;
class FloatingText;

class Monster
	: public Entity
{
public:
	Monster(const float& x, const float& y, sf::Texture& texture, sf::Texture& shadow_texture, const sf::VideoMode& vm, const std::string& monster_name, const float& difficulty_mod);
	virtual ~Monster();

	virtual const bool getSpawned() const;
	virtual const bool getDeadCountdown() const;

	const bool attackPlayer(Player* player, sf::Font* font, const std::vector<sf::Sprite>& obstacles, std::list<Projectile*>& projectiles, std::list<FloatingText*>& floatingTexts);
	const bool sightCollision(const std::vector<sf::Sprite>& obstacles,
		const sf::Vector2f& a_p1, const sf::Vector2f& a_p2);
	void spawn(const float& dt);
	const bool dying(const float& dt);

	void AI(const sf::Vector2f& playerPosition, const float& dt);

	void monsterCollision(const std::list<Monster*>& monsters);

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
	void drawShadow(sf::RenderTarget& target);
private:
	sf::VideoMode vm;
	sf::Sprite shadow;
	sf::Texture shadow_texture;

	bool spawned;
	float deadCountdown;
	float spawnCountdown;
};
#endif