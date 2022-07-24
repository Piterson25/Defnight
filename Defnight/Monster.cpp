#include "Functions.h"
#include "Monster.h"

Monster::Monster()
{
}

Monster::~Monster()
{
}

void Monster::init(float& scale, const std::string& name, const int& attack, const int& attackSpeed, const int& hp, const int& speed,
	const unsigned& gold, const unsigned& xp, const float& PlayerX, const float& PlayerY)
{
	this->scale = scale;
	this->animationTime = 0.f;
	this->step = 0;
	this->predkosc = sf::Vector2f(0.f, 0.f);
	this->attack = 0;
	this->attackCooldown = 0;
	this->attackSpeed = 0;
	this->hp = 0;
	this->speed = 0;
	this->gold = 0;
	this->xp = 0;
	this->punched = false;
	this->punchedTime = 0.f;
	this->attackAni = false;
	this->up = false;
	this->down = false;
	this->left = true;
	this->right = false;
	this->dead = false;
	this->deadCountdown = 255.f;
	this->fired = false;
	this->spawnCountdown = 0.f;
	this->reach = 1;

	this->name = name;
	this->attack = attack;
	this->attackSpeed = attackSpeed;
	this->hp = hp;
	this->speed = speed;
	this->gold = gold;
	this->xp = xp;
	


	this->BlockSize.x = 64 * this->scale;
	this->BlockSize.y = 64 * this->scale;
	this->AStarCooldown = 0.f;
	initNodes(PlayerX, PlayerY);
	
}

void Monster::animation(const float& dt)
{
	if (this->predkosc.x < 0) {
		this->left = true;
		this->right = false;
		this->up = false;
		this->down = false;
	}
	else if (this->predkosc.x > 0) {
		this->left = false;
		this->right = true;
		this->up = false;
		this->down = false;
	}
	else {
		if (this->predkosc.y < 0) {
			this->up = true;
			this->down = false;
			this->left = false;
			this->right = false;
		}
		else if (this->predkosc.y > 0) {
			this->up = false;
			this->down = true;
			this->left = false;
			this->right = false;
		}
	}


	if (this->attackAni && (this->predkosc.x != 0.f || this->predkosc.y != 0.f)) {
		int y = 0;
		if (this->left) y = 10;
		else if (this->right) y = 11;
		else {
			if (this->up) y = 8;
			else if (this->down) y = 9;
		}

		this->animationTime += (this->attackSpeed * 0.2f + 0.8f) * 8.f * dt;
		if (this->animationTime >= 1.f) {
			this->animationTime = 0.f;

			sf::IntRect intRect(this->step, y * 16, 16, 16);
			this->sprite.setTextureRect(intRect);
			this->sprite.setScale(4 * this->scale, 4 * this->scale);

			if (this->step == 112) {
				this->step = 0;
				this->attackAni = false;
				this->fired = false;
			}
			else this->step += 16;
		}
	}
	else if (this->attackAni) {
		int y = 0;
		if (this->left) y = 6;
		else if (this->right) y = 7;
		else {
			if (this->up) y = 4;
			else if (this->down) y = 5;
		}

		this->animationTime += (this->attackSpeed * 0.2f + 0.8f) * 8.f * dt;
		if (this->animationTime >= 1.f) {
			this->animationTime = 0.f;

			sf::IntRect intRect(this->step, y * 16, 16, 16);
			this->sprite.setTextureRect(intRect);
			this->sprite.setScale(4 * this->scale, 4 * this->scale);

			if (this->step == 112) {
				this->step = 0;
				this->attackAni = false;
				this->fired = false;
			}
			else this->step += 16;
		}
	}
	else if (this->predkosc.x != 0.f || this->predkosc.y != 0.f) {
		int y = 0;
		if (this->predkosc.x < 0) y = 2;
		else if (this->predkosc.x > 0) y = 3;
		else {
			if (this->predkosc.y < 0) y = 0;
			else if (this->predkosc.y > 0) y = 1;
		}

		this->animationTime += (this->speed * 0.2f + 0.8f) * 4.f * dt;
		if (this->animationTime >= 1.f) {
			this->animationTime = 0.f;

			sf::IntRect intRect(this->step, y * 16, 16, 16);
			this->sprite.setTextureRect(intRect);
			this->sprite.setScale(4 * this->scale, 4 * this->scale);

			if (this->step == 112) this->step = 0;
			else this->step += 16;
		}
	}
}

void Monster::move()
{
	this->sprite.move(this->predkosc);
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + 52 * this->scale);
}

void Monster::damaged(const float& dt)
{
	if (this->punched && !this->dead) {
		if (this->punchedTime < 1.f) {
			this->punchedTime += 5.f * dt;
			this->sprite.setColor(sf::Color(182, 60, 53));
		}
		if (this->punchedTime >= 1.f) {
			this->punchedTime = 0.f;
			this->sprite.setColor(sf::Color::White);
			this->punched = false;
		}
	}
}

void Monster::spawning(const float& dt)
{
	if (this->sprite.getColor().a < sf::Uint8(255)) {
		this->spawnCountdown += dt;
		if (this->spawnCountdown > 0.003f) {
			this->sprite.setColor(sf::Color(255, 255, 255, sf::Uint8(this->sprite.getColor().a + 1)));
			this->shadow.setColor(sf::Color(255, 255, 255, sf::Uint8(this->shadow.getColor().a + 1)));
		}
	}
}

void Monster::collision(const std::list<Monster>& monsters)
{
	for (const auto& mob : monsters) {
		if (vectorDistance(this->sprite.getPosition(), mob.sprite.getPosition()) < 256 * this->scale) {
			sf::FloatRect monsterBounds = this->sprite.getGlobalBounds();
			sf::FloatRect goblinBounds = mob.sprite.getGlobalBounds();

			sf::FloatRect nextPos = monsterBounds;
			nextPos.left += this->predkosc.x;
			nextPos.top += this->predkosc.y;

			if (goblinBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (monsterBounds.top < goblinBounds.top
					&& monsterBounds.top + monsterBounds.height < goblinBounds.top + goblinBounds.height
					&& monsterBounds.left < goblinBounds.left + goblinBounds.width
					&& monsterBounds.left + monsterBounds.width > goblinBounds.left)
				{
					this->predkosc.y = 0.f;
					this->sprite.setPosition(monsterBounds.left, goblinBounds.top - monsterBounds.height);
				}
				//Gorna kolizja
				else if (monsterBounds.top > goblinBounds.top
					&& monsterBounds.top + monsterBounds.height > goblinBounds.top + goblinBounds.height
					&& monsterBounds.left < goblinBounds.left + goblinBounds.width
					&& monsterBounds.left + monsterBounds.width > goblinBounds.left)
				{
					this->predkosc.y = 0.f;
					this->sprite.setPosition(monsterBounds.left, goblinBounds.top + goblinBounds.height);
				}

				//Prawa kolizja
				else if (monsterBounds.left < goblinBounds.left
					&& monsterBounds.left + monsterBounds.width < goblinBounds.left + goblinBounds.width
					&& monsterBounds.top < goblinBounds.top + goblinBounds.height
					&& monsterBounds.top + monsterBounds.height > goblinBounds.top)
				{
					this->predkosc.x = 0.f;
					this->sprite.setPosition(goblinBounds.left - monsterBounds.width, monsterBounds.top);
				}
				//Lewa kolizja
				else if (monsterBounds.left > goblinBounds.left
					&& monsterBounds.left + monsterBounds.width > goblinBounds.left + goblinBounds.width
					&& monsterBounds.top < goblinBounds.top + goblinBounds.height
					&& monsterBounds.top + monsterBounds.height > goblinBounds.top)
				{
					this->predkosc.x = 0.f;
					this->sprite.setPosition(goblinBounds.left + goblinBounds.width, monsterBounds.top);
				}
			}
		}
	}

}


void Monster::drawing(sf::RenderWindow& window) {
	if (End != nullptr)
	{
		Node* p = End->parent;
		while (p != nullptr && p != Start)
		{
			sf::RectangleShape s;
			s.setSize({ BlockSize.x / 2, BlockSize.y / 2 });
			s.setFillColor(sf::Color::Green);
			s.setPosition(p->x * BlockSize.x + BlockSize.x / 4, p->y * BlockSize.y + BlockSize.y / 4);

			p = p->parent;

			window.draw(s);
		}
	}
}
void Monster::spawnProjectile(std::list<Projectile>& pociski, Projectile& pocisk, const sf::Texture& texture, const sf::Sprite& player, const float& dt)
{
	if (this->name == "Cyklop") {
		pocisk.init(this->scale, "Skala", 3, 1, 1);
		pocisk.texture = texture;
		pocisk.sprite.setTexture(pocisk.texture);
		pocisk.sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
		pocisk.sprite.setScale(4 * this->scale, 4 * this->scale);

		if (this->left)
			pocisk.sprite.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + 16 * this->scale);
		else if (this->right)
			pocisk.sprite.setPosition(this->sprite.getPosition().x + 64 * this->scale, this->sprite.getPosition().y + 16 * this->scale);
		else if (this->up)
			pocisk.sprite.setPosition(this->sprite.getPosition().x + 20 * this->scale, this->sprite.getPosition().y + 16 * this->scale);
		else if (this->down)
			pocisk.sprite.setPosition(this->sprite.getPosition().x + 24 * this->scale, this->sprite.getPosition().y + 36 * this->scale);
		pocisk.calculateVelocity(sf::Vector2f(player.getPosition().x + 32 * this->scale, player.getPosition().y + 32 * this->scale), dt);
		pociski.push_back(pocisk);
	}
}


void Monster::AI(const std::vector<sf::Sprite>& obstacles, const sf::Sprite& player, const std::list<Monster>& potwory, const float& PlayerX, const float& PlayerY, const float& dt)
{
	/*this->AStarCooldown += dt;

	if (this->AStarCooldown > 1.f) {
		this->AStarCooldown = 0.f;

		//sf::Clock c;


		if (sqrt((this->sprite.getPosition().x - PlayerX) * (this->sprite.getPosition().x - PlayerX) +
			(this->sprite.getPosition().y - PlayerY) * (this->sprite.getPosition().y - PlayerY)) > this->reach * 64 * this->scale)
		{
			updateNodes(obstacles, potwory, PlayerX, PlayerY);
		}
		
		//std::cout << c.getElapsedTime().asMilliseconds() << std::endl;

		this->predkosc = (sf::Vector2f(0.f, 0.f));
		sf::Vector2f docelowy_punkt;
		if (End != nullptr)
		{
			Node* p = End->parent;
			docelowy_punkt.x = p->x * BlockSize.x - this->sprite.getPosition().x;
			docelowy_punkt.y = p->y * BlockSize.y - this->sprite.getPosition().y;
		}


		if (docelowy_punkt.x > 0) {
			this->predkosc.x = this->speed * 50.f * dt * this->scale;
		}
		if (docelowy_punkt.x < 0) {
			this->predkosc.x = -(this->speed * 50.f * dt * this->scale);
		}
		if (docelowy_punkt.y > 0) {
			this->predkosc.y = this->speed * 50.f * dt * this->scale;
		}
		if (docelowy_punkt.y < 0) {
			this->predkosc.y = -(this->speed * 50.f * dt * this->scale);
		}

		//std::cout << this->predkosc.x << " " << this->predkosc.y << std::endl;
	}*/


	this->predkosc = sf::Vector2f(0.f, 0.f);
	const float war = (this->speed * 0.2f + 0.8f) * 128.f * dt * this->scale;
	if (PlayerX > this->sprite.getPosition().x) {
		this->predkosc.x += war;
	}
	if (PlayerX < this->sprite.getPosition().x) {
		this->predkosc.x += -(war);
	}
	if (PlayerY > this->sprite.getPosition().y) {
		this->predkosc.y += war;
	}
	if (PlayerY < this->sprite.getPosition().y) {
		this->predkosc.y += -(war);
	}

	if (this->predkosc.x != 0.f && this->predkosc.y != 0.f) {
		this->predkosc.x /= 1.44f;
		this->predkosc.y /= 1.44f;
	}
	

	for (const auto& e : obstacles) {
		if (vectorDistance(this->sprite.getPosition(), e.getPosition()) < 256 * this->scale) {

			sf::FloatRect monsterBounds = this->sprite.getGlobalBounds();
			sf::FloatRect wallBounds = e.getGlobalBounds();

			sf::FloatRect nextPos = monsterBounds;
			nextPos.left += this->predkosc.x;
			nextPos.top += this->predkosc.y;

			if (wallBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (monsterBounds.top < wallBounds.top
					&& monsterBounds.top + monsterBounds.height < wallBounds.top + wallBounds.height
					&& monsterBounds.left < wallBounds.left + wallBounds.width
					&& monsterBounds.left + monsterBounds.width > wallBounds.left)
				{
					this->predkosc.y = 0.f;
					this->sprite.setPosition(monsterBounds.left, wallBounds.top - monsterBounds.height);
				}
				//Gorna kolizja
				else if (monsterBounds.top > wallBounds.top
					&& monsterBounds.top + monsterBounds.height > wallBounds.top + wallBounds.height
					&& monsterBounds.left < wallBounds.left + wallBounds.width
					&& monsterBounds.left + monsterBounds.width > wallBounds.left)
				{
					this->predkosc.y = 0.f;
					this->sprite.setPosition(monsterBounds.left, wallBounds.top + wallBounds.height);
				}
				//Prawa kolizja
				else if (monsterBounds.left < wallBounds.left
					&& monsterBounds.left + monsterBounds.width < wallBounds.left + wallBounds.width
					&& monsterBounds.top < wallBounds.top + wallBounds.height
					&& monsterBounds.top + monsterBounds.height > wallBounds.top)
				{
					this->predkosc.x = 0.f;
					this->sprite.setPosition(wallBounds.left - monsterBounds.width, monsterBounds.top);
				}
				//Lewa kolizja
				else if (monsterBounds.left > wallBounds.left
					&& monsterBounds.left + monsterBounds.width > wallBounds.left + wallBounds.width
					&& monsterBounds.top < wallBounds.top + wallBounds.height
					&& monsterBounds.top + monsterBounds.height > wallBounds.top)
				{
					this->predkosc.x = 0.f;
					this->sprite.setPosition(wallBounds.left + wallBounds.width, monsterBounds.top);
				}

			}
		}
	}

}

void Monster::initNodes(const float& PlayerX, const float& PlayerY)
{
	int x = 0, y = 0;
	if (PlayerX - int(PlayerX) >= 0.5) {
		x = int(PlayerX) + 1;
	}
	else x = int(PlayerX);

	if (PlayerY - int(PlayerY) >= 0.5) {
		y = int(PlayerY) + 1;
	}
	else y = int(PlayerY);


	for (short x = 0; x < CollumsX; ++x)
	{
		Nodes.push_back(new Node[CollumsY]);
		for (short y = 0; y < CollumsY; ++y)
		{
			initDefNode(&Nodes[x][y], x, y);
		}
	}

	for (short x = 0; x < CollumsX; ++x)
		for (short y = 0; y < CollumsY; ++y)
		{
			initNeighBours(&Nodes[x][y]);
		}

	Start = &Nodes[int(x / this->BlockSize.x)][int(y / this->BlockSize.y)];
	End = &Nodes[int(this->sprite.getPosition().x / this->BlockSize.x)][int(this->sprite.getPosition().y / this->BlockSize.y)];
}

void Monster::initNeighBours(Node* node)
{/*
	if (node->x > 0) node->Neighbours.push_back(&Nodes[node->x - 1][node->y]);
	if (node->y > 0) node->Neighbours.push_back(&Nodes[node->x][node->y - 1]);
	if (node->x < CollumsX - 1) node->Neighbours.push_back(&Nodes[node->x + 1][node->y]);
	if (node->y < CollumsY - 1) node->Neighbours.push_back(&Nodes[node->x][node->y + 1]);
	if (node->x > 0 && node->y > 0) node->Neighbours.push_back(&Nodes[node->x - 1][node->y - 1]);*/
}

void Monster::initDefNode(Node* node, const int& x, const int& y)
{
	node->x = x;
	node->y = y;
	node->isWall = false;
	node->isVisited = false;
	node->parent = nullptr;
}

void Monster::AStarAlg()
{
	for (int x = 0; x < CollumsX; x++)
		for (int y = 0; y < CollumsY; y++)
		{
			Nodes[x][y].isVisited = false;
			Nodes[x][y].FCost = 100000.f;
			Nodes[x][y].HCost = 100000.f;
			Nodes[x][y].parent = nullptr;
		}

	auto GetDist = [](Node* P1, Node* P2) { return sqrt(pow(P2->x - P1->x, 2) + pow(P2->y - P1->y, 2)); };

	Node* CurrentNode = Start;
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

		CurrentNode = NodesToTest.front();
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

const sf::Vector2f Monster::getCenterPosition() const
{
	return sf::Vector2f(this->sprite.getPosition().x + 32 * this->scale, this->sprite.getPosition().y + 32 * this->scale);
}

void Monster::updateNodes(std::vector<sf::Sprite>& obstacles, std::list<Monster>& potwory, const float& PlayerX, const float& PlayerY)
{
	/*int x = 0, y = 0;
	if (PlayerX - int(PlayerX) >= 0.5) {
		x = int(PlayerX) + 1;
	}
	else x = int(PlayerX);

	if (PlayerY - int(PlayerY) >= 0.5) {
		y = int(PlayerY) + 1;
	}
	else y = int(PlayerY);


	Start = &Nodes[int(x / this->BlockSize.x)][int(y / this->BlockSize.y)];

	End = &Nodes[int(this->sprite.getPosition().x / this->BlockSize.x)][int(this->sprite.getPosition().y / this->BlockSize.y)];

	for (auto& e : obstacles) {
		Nodes[int(e.getPosition().x / this->BlockSize.x)][int(e.getPosition().y / this->BlockSize.y)].isWall =
			!Nodes[int(e.getPosition().x / this->BlockSize.x)][int(e.getPosition().y / this->BlockSize.y)].isWall;
	}


	for (auto& m : potwory) {
		int x = 0, y = 0;
		if (m.sprite.getPosition().x - int(m.sprite.getPosition().x) >= 0.5) {
			x = int(m.sprite.getPosition().x) + 1;
		}
		else x = int(m.sprite.getPosition().x);

		if (m.sprite.getPosition().y - int(m.sprite.getPosition().y) >= 0.5) {
			y = int(m.sprite.getPosition().y) + 1;
		}
		else y = int(m.sprite.getPosition().y);

		if (int(x / this->BlockSize.x) != int(this->sprite.getPosition().x / this->BlockSize.x) &&
			int(y / this->BlockSize.y) != int(this->sprite.getPosition().y / this->BlockSize.y)) {
			Nodes[int(m.sprite.getPosition().x / this->BlockSize.x)][int(m.sprite.getPosition().y / this->BlockSize.y)].isWall =
				!Nodes[int(m.sprite.getPosition().x / this->BlockSize.x)][int(m.sprite.getPosition().y / this->BlockSize.y)].isWall;
		}
	}

	AStarAlg(); */
}
