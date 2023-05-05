#include "Functions.h"
#include "AIComponent.h"

AIComponent::AIComponent(TileMap* tileMap, sf::VideoMode& vm)
	:vm(vm)
{
	this->Current = nullptr;
	this->End = nullptr;
	this->Start = nullptr;
	this->activateAI = false;
	this->BlockSize.x = calcX(64, vm);
	this->BlockSize.y = calcY(64, vm);
	this->target_position = sf::Vector2f(0.f, 0.f);

	this->initNodes();
	for (size_t i = 0; i < tileMap->getSize(); ++i) {
		Nodes[static_cast<size_t>(tileMap->getPosition(i).x / this->BlockSize.x)][static_cast<size_t>(tileMap->getPosition(i).y / this->BlockSize.y)].isWall = true;
	}
}

AIComponent::~AIComponent()
{
	this->Nodes.clear();
}

void AIComponent::initNodes()
{
	for (size_t x = 0; x < CollumsX; ++x)
	{
		Nodes.push_back(new Node[CollumsY]);
		for (size_t y = 0; y < CollumsY; ++y)
		{
			Nodes[x][y].x = static_cast<int>(x);
			Nodes[x][y].y = static_cast<int>(y);
			Nodes[x][y].isWall = false;
			Nodes[x][y].isVisited = false;
			Nodes[x][y].parent = nullptr;
			Nodes[x][y].FCost = 0.f;
			Nodes[x][y].HCost = 0.f;
		}
	}

	for (size_t x = 0; x < CollumsX; ++x)
	{
		for (size_t y = 0; y < CollumsY; ++y)
		{
			if (Nodes[x][y].x > 0) {
				const int tempX = Nodes[x][y].x - 1;
				Nodes[x][y].Neighbours.push_back(&Nodes[static_cast<size_t>(tempX)][Nodes[x][y].y]);
			}
			if (Nodes[x][y].y > 0) {
				const int tempY = Nodes[x][y].y - 1;
				Nodes[x][y].Neighbours.push_back(&Nodes[Nodes[x][y].x][static_cast<size_t>(tempY)]);
			}
			if (Nodes[x][y].x < CollumsX - 1) {
				const int tempX = Nodes[x][y].x + 1;
				Nodes[x][y].Neighbours.push_back(&Nodes[static_cast<size_t>(tempX)][Nodes[x][y].y]);
			}
			if (Nodes[x][y].y < CollumsY - 1) {
				const int tempY = Nodes[x][y].y + 1;
				Nodes[x][y].Neighbours.push_back(&Nodes[Nodes[x][y].x][static_cast<size_t>(tempY)]);
			}
		}
	}
}

void AIComponent::resetNodes(Entity* e1, Entity* e2, const std::vector<sf::Vector2f>& positions)
{
	Start = &Nodes[int(e2->getPosition().x / this->BlockSize.x)][int(e2->getPosition().y / this->BlockSize.y)];

	End = &Nodes[int(e1->getPosition().x / this->BlockSize.x)][int(e1->getPosition().y / this->BlockSize.y)];

	for (short x = 0; x < CollumsX; ++x)
	{
		for (short y = 0; y < CollumsY; ++y)
		{
			if (Nodes[x][y].isMonster) Nodes[x][y].isWall = false;
			Nodes[x][y].isMonster = false;
			Nodes[x][y].isVisited = false;
			Nodes[x][y].parent = nullptr;
			Nodes[x][y].FCost = 0.f;
			Nodes[x][y].HCost = 0.f;
		}
	}

	for (const auto& p : positions) {
		Nodes[static_cast<size_t>(p.x / this->BlockSize.x)][static_cast<size_t>(p.y / this->BlockSize.y)].isMonster = true;
		Nodes[static_cast<size_t>(p.x / this->BlockSize.x)][static_cast<size_t>(p.y / this->BlockSize.y)].isWall = true;
	}

	AStarAlg();

	this->Current = End;

	this->greens.clear();
}

void AIComponent::AStarAlg()
{
	for (size_t x = 0; x < CollumsX; ++x)
	{
		for (size_t y = 0; y < CollumsY; ++y)
		{
			Nodes[x][y].isVisited = false;
			Nodes[x][y].FCost = 100000.f;
			Nodes[x][y].HCost = 100000.f;
			Nodes[x][y].parent = nullptr;
		}
	}

	auto GetDist = [](Node* P1, Node* P2) { return vectorDistance(float(P2->x), float(P1->x), float(P2->y), float(P2->y)); };

	Start->HCost = 0.0f;
	Start->FCost = float(GetDist(Start, End));

	std::vector<Node*> NodesToTest;
	NodesToTest.push_back(Start);


	while (!NodesToTest.empty())
	{
		std::sort(NodesToTest.begin(), NodesToTest.end(), [](const Node* a, const Node* b) { return a->FCost < b->FCost; });

		while (!NodesToTest.empty() && NodesToTest.front()->isVisited)
			NodesToTest.erase(NodesToTest.begin());

		if (NodesToTest.empty())
			break;

		auto CurrentNode = NodesToTest.front();
		CurrentNode->isVisited = true;


		for (auto nodeNeighbour : CurrentNode->Neighbours)
		{
			if (!nodeNeighbour->isVisited && !nodeNeighbour->isWall)
				NodesToTest.push_back(nodeNeighbour);

			auto BestNode = CurrentNode->HCost + GetDist(CurrentNode, nodeNeighbour);

			if (BestNode < nodeNeighbour->HCost)
			{
				nodeNeighbour->parent = CurrentNode;
				nodeNeighbour->HCost = float(BestNode);

				nodeNeighbour->FCost = nodeNeighbour->HCost + float(GetDist(nodeNeighbour, End));
			}
		}

	}
}

void AIComponent::checkToDisable()
{
	if (this->activateAI) this->activateAI = false;
}

void AIComponent::update(Entity* e1, Entity* e2, const std::vector<sf::Vector2f>& positions, const float& vel)
{
	if (!this->activateAI) {
		resetNodes(e1, e2, positions);
		this->activateAI = true;
	}
	if (this->activateAI) {
		if (Current != nullptr)
		{
			sf::Vector2f target_point(Current->x * BlockSize.x, Current->y * BlockSize.y);

			sf::RectangleShape t;
			t.setSize(sf::Vector2f(16, 16));
			t.setFillColor(sf::Color::Green);
			t.setPosition(sf::Vector2f(Current->x * BlockSize.x + 24, Current->y * BlockSize.y + 24));

			this->greens.push_back(t);

			if (target_point.x > e1->getPosition().x) {
				if (e1->getPosition().x + vel > target_point.x)
					e1->setPosition(target_point.x, e1->getPosition().y);
				else e1->setVeloctiy(sf::Vector2f(vel, e1->getVelocity().y));
			}
			else if (target_point.x < e1->getPosition().x) {
				if (e1->getPosition().x - vel < target_point.x)
					e1->setPosition(target_point.x, e1->getPosition().y);
				else e1->setVeloctiy(sf::Vector2f(-vel, e1->getVelocity().y));
			}
			if (target_point.y > e1->getPosition().y) {
				if (e1->getPosition().y + vel > target_point.y)
					e1->setPosition(e1->getPosition().x, target_point.y);
				else e1->setVeloctiy(sf::Vector2f(e1->getVelocity().x, vel));
			}
			else if (target_point.y < e1->getPosition().y) {
				if (e1->getPosition().y - vel < target_point.y)
					e1->setPosition(e1->getPosition().x, target_point.y);
				else e1->setVeloctiy(sf::Vector2f(e1->getVelocity().x, -vel));
			}

			if (int(e1->getPosition().x / calcX(64, vm)) == int(Current->x) &&
				int(e1->getPosition().y / this->BlockSize.y) == int(Current->y)) {
				if (Current->parent != nullptr) {
					Current = Current->parent;
				}
				else {
					resetNodes(e1, e2, positions);
					this->activateAI = true;
				}
			}
		}
		else {
			this->activateAI = false;
		}
	}
}

void AIComponent::draw(sf::RenderTarget& target)
{
	for(auto& e : greens)
		target.draw(e);
}
