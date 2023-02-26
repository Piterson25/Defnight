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

	this->gold = 0;
	this->monsterSize = 1;

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

	this->playedSound = false;

	this->AI = new AIComponent(tileMap, this->vm);

	if (this->name == "goblin") {
		this->attack = static_cast<uint32_t>(1 * difficulty_mod);
		this->attackSpeed = 2;
		this->HP = static_cast<uint32_t>(3 * difficulty_mod);
		this->maxHP = static_cast<uint32_t>(3 * difficulty_mod);
		this->speed = 2;
		this->gold = static_cast<uint32_t>(1 * wave_mod);
		this->XP = static_cast<uint32_t>(5 * wave_mod);
	}
	else if (this->name == "spider") {
		this->attack = static_cast<uint32_t>(2 * difficulty_mod);
		this->attackSpeed = 3;
		this->HP = static_cast<uint32_t>(7 * difficulty_mod);
		this->maxHP = static_cast<uint32_t>(7 * difficulty_mod);
		this->speed = 3;
		this->gold = static_cast<uint32_t>(2 * wave_mod);
		this->XP = static_cast<uint32_t>(10 * wave_mod);
	}
	else if (this->name == "orc") {
		this->attack = static_cast<uint32_t>(3 * difficulty_mod);
		this->attackSpeed = 2;
		this->HP = static_cast<uint32_t>(12 * difficulty_mod);
		this->maxHP = static_cast<uint32_t>(12 * difficulty_mod);
		this->speed = 2;
		this->gold = static_cast<uint32_t>(3 * wave_mod);
		this->XP = static_cast<uint32_t>(17 * wave_mod);
	}
	else if (this->name == "cyclope") {
		this->attack = static_cast<uint32_t>(4 * difficulty_mod);
		this->attackSpeed = 1;
		this->HP = static_cast<uint32_t>(18 * difficulty_mod);
		this->maxHP = static_cast<uint32_t>(18 * difficulty_mod);
		this->speed = 1;
		this->gold = static_cast<uint32_t>(4 * wave_mod);
		this->XP = static_cast<uint32_t>(26 * wave_mod);
	}
}

Monster::~Monster()
{
	this->AI->~AIComponent();
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
					projectileSystem->addProjectile("stone", this->getPosition().x + calcX(24, vm), this->getPosition().y + calcY(36, vm), this->getAttack(), 1, 2, sf::Vector2f(player->getPosition().x + calcX(32, vm), player->getPosition().y + calcY(32, vm)), 0, false);
					this->isAttacking = false;

					if (!this->playedSound) {
						soundEngine->addSound("punch");
						this->playedSound = true;
					}
				}
			}
		}
	}
	else if (distance <= this->getReach() * calcX(static_cast<float>(32 * this->monsterSize), vm)) {
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

void Monster::calculateAI(TileMap* tileMap, Player* player, const std::vector<sf::Vector2f>& positions, const float& dt)
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

		this->AI->checkToDisable();
	}
	else if (this->name != "minotaur") {
		this->AI->update(this, player, positions, vel);
	}

	if (this->velocity.x != 0.f && this->velocity.y != 0.f) {
		this->velocity.x /= 1.44f;
		this->velocity.y /= 1.44f;
	}
}

void Monster::update(const float& dt, ProjectileSystem* projectileSystem, SoundEngine* soundEngine)
{
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(static_cast<float>(52 * this->monsterSize), this->vm));
	if (this->playedSound && this->frame != 80) {
		this->playedSound = false;
	}
}

void Monster::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
	//this->AI->draw(target);
}

void Monster::drawShadow(sf::RenderTarget& target)
{
	target.draw(this->shadow);
}
