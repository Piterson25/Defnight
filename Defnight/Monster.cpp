#include "stdafx.h"
#include "Functions.h"
#include "Monster.h"

Monster::Monster(const sf::VideoMode& vm, const std::string& monster_name, sf::Texture& texture, sf::Texture& shadow_texture, TileMap* tileMap, 
	const float& x, const float& y, const float& difficulty_mod, const float& wave_mod)
{
	this->vm = vm;
	this->name = monster_name;
	this->texture = texture;
	this->shadow_texture = shadow_texture;

	this->monsterSize = 1;
	if (this->name == "minotaur") this->monsterSize = 2;

	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(sf::IntRect(0, 32 * this->monsterSize, 16 * this->monsterSize, 16 * this->monsterSize));
	this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
	this->sprite.setPosition(x, y);
	this->sprite.setColor(sf::Color(255, 255, 255, 0));

	this->shadow.setTexture(this->shadow_texture);
	this->shadow.setScale(calcScale(static_cast<float>(4 * this->monsterSize), vm), calcScale(static_cast<float>(4 * this->monsterSize), vm));
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(static_cast<float>(52 * this->monsterSize), vm));
	this->shadow.setColor(sf::Color(255, 255, 255, 0));

	this->reach = 1;
	this->spawnCountdown = 0.f;
	this->deadCountdown = 0.f;
	this->spawned = false;
	this->activateAI = false;

	this->playedSound = false;

	this->BlockSize.x = calcX(64, vm);
	this->BlockSize.y = calcY(64, vm);

	this->Current = nullptr;

	this->initNodes();
	for (size_t i = 0; i < tileMap->getSize(); ++i) {
		Nodes[static_cast<size_t>(tileMap->getPosition(i).x / this->BlockSize.x)][static_cast<size_t>(tileMap->getPosition(i).y / this->BlockSize.y)].isWall = true;
	}
	

	if (this->name == "goblin") {
		this->attack = static_cast<unsigned>(1 * difficulty_mod * wave_mod);
		this->attackSpeed = 2;
		this->HP = static_cast<unsigned>(3 * difficulty_mod * wave_mod);
		this->speed = 2;
		this->gold = static_cast<unsigned>(1 * wave_mod);
		this->XP = static_cast<unsigned>(5 * wave_mod);
	}
	else if (this->name == "spider") {
		this->attack = static_cast<unsigned>(2 * difficulty_mod * wave_mod);
		this->attackSpeed = 3;
		this->HP = static_cast<unsigned>(7 * difficulty_mod * wave_mod);
		this->speed = 3;
		this->gold = static_cast<unsigned>(2 * wave_mod);
		this->XP = static_cast<unsigned>(10 * wave_mod);
	}
	else if (this->name == "orc") {
		this->attack = static_cast<unsigned>(3 * difficulty_mod * wave_mod);
		this->attackSpeed = 2;
		this->HP = static_cast<unsigned>(12 * difficulty_mod * wave_mod);
		this->speed = 2;
		this->gold = static_cast<unsigned>(3 * wave_mod);
		this->XP = static_cast<unsigned>(17 * wave_mod);
	}
	else if (this->name == "cyclope") {
		this->attack = static_cast<unsigned>(4 * difficulty_mod * wave_mod);
		this->attackSpeed = 1;
		this->HP = static_cast<unsigned>(18 * difficulty_mod * wave_mod);
		this->speed = 1;
		this->gold = static_cast<unsigned>(4 * wave_mod);
		this->XP = static_cast<unsigned>(26 * wave_mod);
	}
	else if (this->name == "minotaur") {
		this->attack = static_cast<unsigned>(7 * difficulty_mod * wave_mod);
		this->attackSpeed = 1;
		this->HP = static_cast<unsigned>(69 * difficulty_mod * wave_mod);
		this->speed = 1;
		this->gold = static_cast<unsigned>(25 * wave_mod);
		this->XP = static_cast<unsigned>(200 * wave_mod);
	}
}

Monster::~Monster()
{
}

const uint32_t Monster::getGold() const
{
	return this->gold;
}

const bool Monster::getSpawned() const
{
	return this->spawned;
}

const bool Monster::getDeadCountdown() const
{
	if (this->deadCountdown >= 0.25f) return true;
	return false;
}

const bool Monster::attackPlayer(Player* player, TileMap* tileMap, ProjectileSystem* projectileSystem, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine)
{
	const float distance = this->attackDistance(player, this);

	if (this->name == "cyclope" && distance <= this->reach * 8.f * calcX(64, vm)) {
		if (!sightCollision(tileMap, sf::Vector2f(this->getPosition().x + calcX(24, vm), this->getPosition().y + calcY(36, vm)), player->getCenter())) {
			this->doAttack();
			if (!player->isDead() && !player->getPunched() && this->getIsAttacking() && this->getFrame() == 80) {
				if (this->getAttack() > 0) {
					projectileSystem->addProjectile("stone", this->getPosition().x + calcX(24, vm), this->getPosition().y + calcY(36, vm), this->getAttack(), 1, 1, sf::Vector2f(player->getPosition().x + calcX(32, vm), player->getPosition().y + calcY(32, vm)));
					this->isAttacking = false;

					if (!this->playedSound) {
						soundEngine->addSound("punch");
						this->playedSound = true;
					}
				}
			}
		}
	}
	else if ((hasVelocity() && distance <= this->getReach() * calcX(static_cast<float>(32 * this->monsterSize), vm)) || (!hasVelocity() && distance <= this->getReach() * calcX(static_cast<float>(48 * this->monsterSize), vm))) {
		this->doAttack();
		if (!player->isDead() && !player->getPunched() && this->getIsAttacking() && this->getFrame() == 80 * this->monsterSize) {
			int Lattack = static_cast<int>(round(this->attack - (this->attack * player->getArmor() * 0.05f)));
			
			if (Lattack > 0) {
				floatingTextSystem->addFloatingText(std::to_string(-Lattack), calcChar(16, vm), player->getPosition().x + calcX(48, vm), player->getPosition().y + calcY(32, vm), sf::Color(228, 92, 95), false);
				if (static_cast<int>(player->getHP() - Lattack) < 0) player->setHP(0);
				else player->setHP(player->getHP() - Lattack);

				if (!this->playedSound) {
					soundEngine->addSound("punch");
					this->playedSound = true;
				}

				player->punch();

				return true;
			}
		}
	}

	return false;
}

const bool Monster::sightCollision(TileMap* tileMap, const sf::Vector2f& a_p1, const sf::Vector2f& a_p2)
{
	for (size_t i = 0; i < tileMap->getSize(); ++i) {
		if (vectorDistance(this->sprite.getPosition(), tileMap->getPosition(i)) <= 20.f * calcX(32, vm)) {
			if (sight(tileMap->getGlobalBounds(i), a_p1, a_p2)) {
				return true;
			}
		}
	}
	return false;
}

void Monster::spawn(const float& dt)
{
	if (this->spawnCountdown < 0.5f) {
		this->sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(this->spawnCountdown * 510.f)));
		this->shadow.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(this->spawnCountdown * 510.f)));
		this->spawnCountdown += dt;
	}
	if (this->spawnCountdown >= 0.5f) {
		this->spawned = true;
		this->sprite.setColor(sf::Color::White);
		this->shadow.setColor(sf::Color::White);
	}
}

const bool Monster::dying(const float& dt)
{
	if (this->deadCountdown < 0.25f) {
		this->sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255.f - (this->deadCountdown * 1020.f))));
		this->shadow.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255.f - (this->deadCountdown * 1020.f))));
		this->deadCountdown += dt;
	}
	if (this->deadCountdown >= 0.25f) {
		this->sprite.setColor(sf::Color(255, 255, 255, 0));
		this->shadow.setColor(sf::Color(255, 255, 255, 0));
		return true;
	}
	return false;
}

void Monster::setGold(const uint32_t& gold)
{
	this->gold = gold;
}

void Monster::resetNodes(Player* player)
{
	Start = &Nodes[int(player->getPosition().x / this->BlockSize.x)][int(player->getPosition().y / this->BlockSize.y)];

	End = &Nodes[int(this->sprite.getPosition().x / this->BlockSize.x)][int(this->sprite.getPosition().y / this->BlockSize.y)];

	for (short x = 0; x < CollumsX; ++x)
	{
		for (short y = 0; y < CollumsY; ++y)
		{
			Nodes[x][y].isVisited = false;
			Nodes[x][y].parent = nullptr;
			Nodes[x][y].FCost = 0.f;
			Nodes[x][y].HCost = 0.f;
		}
	}

	AStarAlg();

	this->activateAI = true;
	this->Current = End;

	this->greens.clear();
}

void Monster::AI(TileMap* tileMap, Player* player, const float& dt)
{
	this->velocity = sf::Vector2f(0.f, 0.f);
	const float vel = ((this->speed * 0.2f + 0.8f) * 2 * this->sprite.getGlobalBounds().width) * dt;

	if (!sightCollision(tileMap, this->getCenter(), player->getCenter())) {

		if (player->getCenter().x > this->getCenter().x)
			this->velocity.x += vel;
		else if (player->getCenter().x < this->getCenter().x)
			this->velocity.x += -(vel);
		if (player->getCenter().y > this->getCenter().y)
			this->velocity.y += vel;
		else if (player->getCenter().y < this->getCenter().y)
			this->velocity.y += -(vel);

		if (this->activateAI) this->activateAI = false;
	}
	else {
		if (!this->activateAI) {
			resetNodes(player);
		}
		if (this->activateAI) {
			if (Current != nullptr)
			{
				sf::Vector2f docelowy_punkt(Current->x * BlockSize.x, Current->y * BlockSize.y);

				sf::RectangleShape t;
				t.setSize(sf::Vector2f(16, 16));
				t.setFillColor(sf::Color::Green);
				t.setPosition(sf::Vector2f(Current->x * BlockSize.x + 24, Current->y * BlockSize.y + 24));

				this->greens.push_back(t);

				if (docelowy_punkt.x > this->sprite.getPosition().x) {
					if (this->sprite.getPosition().x + vel > docelowy_punkt.x)
						this->sprite.setPosition(docelowy_punkt.x, sprite.getPosition().y);
					else this->velocity.x = vel;
				}
				else if (docelowy_punkt.x < this->sprite.getPosition().x) {
					if (this->sprite.getPosition().x - vel < docelowy_punkt.x)
						this->sprite.setPosition(docelowy_punkt.x, sprite.getPosition().y);
					else this->velocity.x = -(vel);
				}
				if (docelowy_punkt.y > this->sprite.getPosition().y) {
					if (this->sprite.getPosition().y + vel > docelowy_punkt.y)
						this->sprite.setPosition(sprite.getPosition().x, docelowy_punkt.y);
					else this->velocity.y = vel;
				}
				else if (docelowy_punkt.y < this->sprite.getPosition().y) {
					if (this->sprite.getPosition().y - vel < docelowy_punkt.y)
						this->sprite.setPosition(sprite.getPosition().x, docelowy_punkt.y);
					else this->velocity.y = -(vel);
				}
				
				if (int(this->sprite.getPosition().x / this->BlockSize.x) == int(Current->x) &&
					int(this->sprite.getPosition().y / this->BlockSize.y) == int(Current->y)) {
					if (Current->parent != nullptr) {
						Current = Current->parent;
					}
					else resetNodes(player);
				}
			}
			else {
				this->activateAI = false;
			}
		}

	}

	if (this->velocity.x != 0.f && this->velocity.y != 0.f) {
		this->velocity.x /= 1.44f;
		this->velocity.y /= 1.44f;
	}
}

void Monster::update(const float& dt)
{
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(static_cast<float>(52 * this->monsterSize), this->vm));
	if (this->playedSound && this->frame != 80) {
		this->playedSound = false;
	}
}

void Monster::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
	//for(auto& e : greens)
	//target.draw(e);
}

void Monster::drawShadow(sf::RenderTarget& target)
{
	target.draw(this->shadow);
}

void Monster::initNodes()
{
	for (short x = 0; x < CollumsX; ++x)
	{
		Nodes.push_back(new Node[CollumsY]);
		for (short y = 0; y < CollumsY; ++y)
		{
			Nodes[x][y].x = x;
			Nodes[x][y].y = y;
			Nodes[x][y].isWall = false;
			Nodes[x][y].isVisited = false;
			Nodes[x][y].parent = nullptr;
			Nodes[x][y].FCost = 0.f;
			Nodes[x][y].HCost = 0.f;
		}
	}

	for (short x = 0; x < CollumsX; ++x)
		for (short y = 0; y < CollumsY; ++y)
		{
			if (Nodes[x][y].x > 0) Nodes[x][y].Neighbours.push_back(&Nodes[static_cast<size_t>(Nodes[x][y].x - 1)][Nodes[x][y].y]);
			if (Nodes[x][y].y > 0) Nodes[x][y].Neighbours.push_back(&Nodes[Nodes[x][y].x][Nodes[x][y].y - 1]);
			if (Nodes[x][y].x < CollumsX - 1) Nodes[x][y].Neighbours.push_back(&Nodes[static_cast<size_t>(Nodes[x][y].x + 1)][Nodes[x][y].y]);
			if (Nodes[x][y].y < CollumsY - 1) Nodes[x][y].Neighbours.push_back(&Nodes[Nodes[x][y].x][Nodes[x][y].y + 1]);
		}

	Start = nullptr;
	End = nullptr;
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

	auto GetDist = [](Node* P1, Node* P2) { return vectorDistance(float(P2->x), float(P1->x), float(P2->y), float(P2->y)); };

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
