#include "stdafx.h"
#include "Functions.h"
#include "Boss.h"

Boss::Boss(const sf::VideoMode& vm, const std::string& monster_name, sf::Texture& texture, sf::Texture& shadow_texture, TileMap* tileMap, 
	const float& x, const float& y, const float& difficulty_mod, const float& wave_mod, const uint32_t& monsterSize)
{
	this->vm = vm;
	this->name = monster_name;
	this->texture = texture;
	this->shadow_texture = shadow_texture;

	this->monsterSize = monsterSize;

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

	if (this->name == "minotaur") {
		this->attack = static_cast<uint32_t>(7 * difficulty_mod);
		this->attackSpeed = 2;
		this->HP = static_cast<uint32_t>(69 * difficulty_mod);
		this->maxHP = static_cast<uint32_t>(69 * difficulty_mod);
		this->speed = 1;
		this->gold = static_cast<uint32_t>(50 * wave_mod);
		this->XP = static_cast<uint32_t>(200 * wave_mod);
	}
}

Boss::~Boss()
{
	this->Nodes.clear();
}

void Boss::update(const float& dt)
{
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(static_cast<float>(52 * this->monsterSize), this->vm));
	if (this->playedSound && this->frame != 80) {
		this->playedSound = false;
	}
}

void Boss::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
	for (auto& e : greens)
		target.draw(e);
}

void Boss::drawShadow(sf::RenderTarget& target)
{
	target.draw(this->shadow);
}
