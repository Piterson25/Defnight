#ifndef MONSTER_H
#define MONSTER_H

#include "Projectile.h"
#include "Player.h"
#include "Entity.h"
#include "FloatingText.h"
#include "SoundEngine.h"

class Projectile;
class Player;
class FloatingText;
class SoundEngine;

class Monster
	: public Entity
{
public:
	Monster(const float& x, const float& y, sf::Texture& texture, sf::Texture& shadow_texture, const std::vector<Tile*>& tiles, const sf::VideoMode& vm, const std::string& monster_name, const float& difficulty_mod, const float& wave_mod);
	virtual ~Monster();

	virtual const uint16_t getGold() const;
	virtual const uint16_t getAttack() const;
	virtual const bool getSpawned() const;
	virtual const bool getDeadCountdown() const;

	const bool attackPlayer(Player* player, sf::Font* font, const std::vector<Tile*>& tiles, std::list<Projectile*>& projectiles, std::list<FloatingText*>& floatingTexts, SoundEngine* soundEngine);
	const bool sightCollision(const std::vector<Tile*>& tiles, const sf::Vector2f& a_p1, const sf::Vector2f& a_p2);
	void spawn(const float& dt);
	const bool dying(const float& dt);

	void setGold(const uint16_t& gold);
	void setAttack(const uint16_t& attack);

	void AI(const std::vector<Tile*>& tiles, Player* player, const float& dt);

	void monsterCollision(const std::list<Monster*>& monsters);

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
	void drawShadow(sf::RenderTarget& target);
private:
	sf::Sprite shadow;
	sf::Texture shadow_texture;

	uint16_t gold;
	uint16_t attack;
	uint8_t monsterSize;

	bool spawned;
	float deadCountdown;
	float spawnCountdown;

	sf::Vector2f BlockSize;
	const size_t CollumsX = 32;
	const size_t CollumsY = 32;

	bool activateAI;

	struct Node
	{
		bool isWall = false;
		bool isVisited = false;
		float FCost = 0.f;
		float HCost = 0.f;
		int x = 0;
		int y = 0;
		std::vector<Node*> Neighbours = {};
		Node* parent = nullptr;
	};

	std::vector<Node*> Nodes;
	Node* Start;
	Node* End;
	Node* Current;

	std::vector<sf::RectangleShape> greens;

	void resetNodes(Player* player);
	void initNodes();
	void AStarAlg();

	bool playedSound;
};
#endif