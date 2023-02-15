#ifndef MONSTER_H
#define MONSTER_H

#include "Player.h"
#include "Entity.h"
#include "ProjectileSystem.h"
#include "FloatingTextSystem.h"
#include "SoundEngine.h"
#include "TileMap.h"

class Player;
class ProjectileSystem;
class FloatingTextSystem;
class SoundEngine;
class TileMap;

class Monster
	: public Entity
{
public:
	Monster();
	Monster(const sf::VideoMode& vm, const std::string& monster_name, sf::Texture& texture, sf::Texture& shadow_texture, TileMap* tileMap, const float& x, const float& y, const float& difficulty_mod, const float& wave_mod);
	virtual ~Monster();

	virtual const uint32_t getGold() const;
	virtual const bool getSpawned() const;
	virtual const bool getDeadCountdown() const;

	const bool attackPlayer(Player* player, TileMap* tileMap, ProjectileSystem* projectileSystem, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine);
	const bool sightCollision(TileMap* tileMap, const sf::Vector2f& a_p1, const sf::Vector2f& a_p2);
	void spawn(const float& dt);
	const bool dying(const float& dt);

	void setGold(const uint32_t& gold);

	void AI(TileMap* tileMap, Player* player, const std::vector<sf::Vector2f>& positions, const float& dt);

	virtual void update(const float& dt);
	virtual void draw(sf::RenderTarget& target);
	virtual void drawShadow(sf::RenderTarget& target);
protected:
	sf::Sprite shadow;
	sf::Texture shadow_texture;

	uint32_t gold;
	uint32_t monsterSize;

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
		bool isMonster = false;
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

	void resetNodes(Player* player, const std::vector<sf::Vector2f>& positions);
	void initNodes();
	void AStarAlg();

	bool playedSound;
};
#endif