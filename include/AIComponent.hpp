#ifndef AICOMPONENT_HPP
#define AICOMPONENT_HPP

#include "TileMap.hpp"
#include "Entity.hpp"

class TileMap;
class Entity;

class AIComponent
{
public:
	AIComponent(TileMap* tileMap, sf::VideoMode& vm);
	virtual ~AIComponent();

	void checkToDisable();
	void update(Entity* e1, Entity* e2, const std::vector<sf::Vector2f>& positions, const float& vel);

	virtual void draw(sf::RenderTarget& target);
private:
	void initNodes();
	void resetNodes(Entity* e1, Entity* e2, const std::vector<sf::Vector2f>& positions);
	void AStarAlg();

	sf::Vector2f position;
	sf::Vector2f target_position;
	sf::VideoMode& vm;

	bool activateAI;

	sf::Vector2f BlockSize;
	const size_t CollumsX = 32;
	const size_t CollumsY = 32;

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
};

#endif