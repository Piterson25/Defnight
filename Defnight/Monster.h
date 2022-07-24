#pragma once
#include "Projectile.h"

class Monster
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
	int hp;
	unsigned speed;
	unsigned reach;
	unsigned gold;
	unsigned xp;

	bool fired;
	bool dead;
	float deadCountdown;
	float spawnCountdown;


	sf::Vector2f BlockSize;
	const size_t CollumsX = 32;
	const size_t CollumsY = 32;

	struct Node
	{
		bool isWall = false;
		bool isVisited = false;
		float FCost;
		float HCost;
		int x;
		int y;
		std::vector<Node*> Neighbours;
		Node* parent;
	};

	std::vector<Node*> Nodes;
	Node* Start;
	Node* End;

	float AStarCooldown;






	Monster();
	virtual ~Monster();

	void init(float& scale, const std::string& name, const int& attack, const int& attackSpeed, const int& hp, const int& speed,
		const unsigned& gold, const unsigned& xp, const float& PlayerX, const float& PlayerY);
	void collision(const std::list<Monster>& monsters);

	void animation(const float& dt);
	void move();

	void damaged(const float& dt);
	void spawning(const float& dt);

	void spawnProjectile(std::list<Projectile>& pociski, Projectile& pocisk, const sf::Texture& texture, const sf::Sprite& player, const float& dt);

	void AI(const std::vector<sf::Sprite>& obstacles, const sf::Sprite& player, const std::list<Monster>& potwory, const float& PlayerX, const float& PlayerY, const float& dt);

	void initNodes(const float& PlayerX, const float& PlayerY);
	void initNeighBours(Node* node);
	void initDefNode(Node* node, const int& x, const int& y);

	void updateNodes(std::vector<sf::Sprite>& obstacles, std::list<Monster>& potwory, const float& PlayerX, const float& PlayerY);
	void AStarAlg();

	virtual const sf::Vector2f getCenterPosition() const;


	void drawing(sf::RenderWindow& window);
};

