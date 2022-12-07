#include "stdafx.h"
#include "Functions.h"
#include "Player.h"

Player::Player(const sf::VideoMode& vm, const std::string& hero_name, const float& x, const float& y)
{
	this->vm = vm;
	this->name = hero_name;

	this->texture.loadFromFile("external/assets/heroes/" + hero_name + ".png");
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(sf::IntRect(0, 32, 16, 16));
	this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
	this->sprite.setPosition(x, y);
	this->sprite.setColor(sf::Color(255, 255, 255, 0));

	this->shadow_texture.loadFromFile("external/assets/entity_shadow.png");
	this->shadow.setTexture(this->shadow_texture);
	this->shadow.setScale(calcScale(4, vm), calcScale(4, vm));
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(52, vm));
	this->shadow.setColor(sf::Color(255, 255, 255, 0));

	this->ability_texture.loadFromFile("external/assets/abilities.png");
	this->ability.setTexture(this->ability_texture);
	this->ability.setScale(calcScale(4, vm), calcScale(4, vm));
	this->ability.setTextureRect(sf::IntRect(0, 0, 16, 16));
	this->ability.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(52, vm));
	this->ability.setColor(sf::Color(255, 255, 255, 0));

	this->playedSound = false;

	this->level = 1;
	this->lastMaxXP = 0;
	this->maxXP = 40;
	this->reg = 0;
	this->reach = 1;
	this->regCooldown = 0.f;
	this->criticalChance = 0;
	this->spawned = false;
	this->spawnCountdown = 0.f;
	this->isRegenerating = false;
	this->isLeveling = false;
	this->abilityActive = false;
	this->abilityMaxTime = 0.f;
	this->abilityTime = 0.f;
	this->abilityCooldown = 0.f;

	this->gold = 0;

	this->kills = 0;

	if (this->name == "warrior") {
		this->attack = 5;
		this->attackSpeed = 3;
		this->maxHP = 10;
		this->HP = 10;
		this->reg = 1;
		this->speed = 4;
		this->armor = 3;
		this->criticalChance = 10;
	}
}

Player::~Player()
{

}

const uint32_t Player::getGold() const
{
	return this->gold;
}

const uint32_t Player::getArmor() const
{
	return this->armor;
}

const uint32_t Player::getReg() const
{
	return this->reg;
}

const uint32_t Player::getLevel() const
{
	return this->level;
}

const uint32_t Player::getMaxXP() const
{
	return this->maxXP;
}

const uint32_t Player::getLastMaxXP() const
{
	return this->lastMaxXP;
}

const uint32_t Player::getCriticalChance() const
{
	return this->criticalChance;
}

const uint32_t Player::getKills() const
{
	return this->kills;
}

const bool Player::getRegenerating() const
{
	return this->isRegenerating;
}

const bool Player::getLeveling() const
{
	return this->isLeveling;
}

const bool Player::getSpawned() const
{
	return this->spawned;
}

const bool Player::getAbilityActive() const
{
	return this->abilityActive;
}

const float Player::getAbilityCooldown() const
{
	return this->abilityCooldown;
}

const float Player::getAbilityTime() const
{
	return this->abilityTime;
}

const float Player::getAbilityMaxTime() const
{
	return this->abilityMaxTime;
}

void Player::setGold(const uint32_t& gold)
{
	this->gold = gold;
}

void Player::setArmor(const uint32_t& armor)
{
	this->armor = armor;
}

void Player::setReg(const uint32_t& reg)
{
	this->reg = reg;
}

void Player::setCriticalChance(const uint32_t& criticalChance)
{
	this->criticalChance = criticalChance;
}

void Player::setKills(const uint32_t& kills)
{
	this->kills = kills;
}

void Player::setIsRegenerating(const bool& isRegenerating)
{
	this->isRegenerating = isRegenerating;
}

void Player::setIsLeveling(const bool& isLeveling)
{
	this->isLeveling = isLeveling;
}

void Player::setAbilityCooldown(const float& abilityCooldown)
{
	this->abilityCooldown = abilityCooldown;
}

void Player::setAbilityTime(const float& abilityTime)
{
	this->abilityTime = abilityTime;
}

void Player::setAbilityMaxTime(const float& abilityMaxTime)
{
	this->abilityMaxTime = abilityMaxTime;
}

void Player::attackMonster(MonsterSystem* monsterSystem, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine)
{
	monsterSystem->playerAttack(this, floatingTextSystem, soundEngine, this->playedSound);
}

const bool Player::addXP(const uint32_t& monsterXP)
{
	this->setIsLeveling(true);
	this->setXP(this->getXP() + monsterXP);

	bool addedlevel = false;

	while (this->XP >= this->maxXP) {
		this->level++;
		this->lastMaxXP = this->maxXP;
		this->maxXP += this->maxXP + this->level * 10;
		addedlevel = true;
	}
	if (addedlevel) return true;
	return false;
}

void Player::spawn(const float& dt)
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

void Player::controls(const std::unordered_map<std::string, int>& keybinds, const float& dt)
{
	this->velocity = sf::Vector2f(0.f, 0.f);

	const float vel = ((this->speed * 0.2f + 0.8f) * 2.f * this->sprite.getGlobalBounds().width) * dt;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_UP"))))
		this->velocity.y += -(vel);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_DOWN"))))
		this->velocity.y += vel;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_LEFT"))))
		this->velocity.x += -(vel);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_RIGHT"))))
		this->velocity.x += vel;

	if (this->name == "scout" && this->abilityActive)
		this->velocity *= 1.3f;
}

const bool Player::regeneration(const float& dt)
{
	if (this->regCooldown < 1.f && this->HP < this->maxHP)
		this->regCooldown += (this->reg * 0.4f + 0.8f) / 4.f * dt;

	if (this->regCooldown >= 1.f) {
		this->regCooldown = 0.f;
		if (this->HP < this->maxHP) {
			this->HP++;
			return true;
		}
	}
	return false;
}

void Player::abilityCounter(const float& dt)
{
	if (this->abilityMaxTime > 0.f && this->level >= 5) {
		if (this->abilityCooldown < this->abilityMaxTime) {
			this->abilityCooldown += dt;
		}

		if (this->abilityCooldown >= this->abilityTime && this->abilityActive) {
			this->abilityActive = false;
			if (this->name == "knight") {
				this->armor -= 5;
			}
		}
		else if (this->abilityCooldown > this->abilityMaxTime) {
			this->abilityCooldown = this->abilityMaxTime;
		}
	}
}

const bool Player::checkIfAbility()
{
	if (this->level >= 5) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->abilityCooldown == this->abilityMaxTime) {
			this->abilityActive = true;
			this->abilityCooldown = 0.f;

			if (this->name == "knight") {
				this->ability.setTextureRect(sf::IntRect(0, 0, 16, 16));
			}
			else if (this->name == "scout") this->ability.setTextureRect(sf::IntRect(16, 0, 16, 16));
			this->ability.setColor(sf::Color(255, 255, 255, 128));
			return true;
		}
	}
	return false;
}

void Player::doAbility(const sf::Vector2f& coords, ProjectileSystem* projectileSystem, SoundEngine* soundEngine)
{
	if (this->name == "ninja") {
		projectileSystem->addProjectile("shuriken", this->getPosition().x + calcX(32, vm), this->getPosition().y + calcY(32, vm), 3, 3, 4, coords, 0);
		soundEngine->addSound("shuriken");
	}
	else if (this->name == "knight") {
		this->armor += 5;
		soundEngine->addSound("ability");
	}
	else if (this->name == "scout") {
		soundEngine->addSound("ability");
	}
	else if (this->name == "master") {
		projectileSystem->addProjectile("shuriken", this->getPosition().x + calcX(32, vm), this->getPosition().y + calcY(32, vm), 5, 4, 4, coords, 0);
		soundEngine->addSound("shuriken");
		projectileSystem->addProjectile("shuriken", this->getPosition().x + calcX(32, vm), this->getPosition().y + calcY(32, vm), 5, 4, 4, coords, -45.f);
		soundEngine->addSound("shuriken");
		projectileSystem->addProjectile("shuriken", this->getPosition().x + calcX(32, vm), this->getPosition().y + calcY(32, vm), 5, 4, 4, coords, 45.f);
		soundEngine->addSound("shuriken");
	}
	else if (this->name == "bomber") {
		projectileSystem->addProjectile("bomb", this->getPosition().x + calcX(32, vm), this->getPosition().y + calcY(32, vm), 7, 1, 3, coords, 0);
		soundEngine->addSound("shuriken");
	}
}

void Player::whooshSound(SoundEngine* soundEngine)
{
	if (this->isAttacking && this->frame == 80 && !this->playedSound) {
		soundEngine->addSound("whoosh");
		this->playedSound = true;
	}
	if ( this->playedSound && this->frame != 80) {
		this->playedSound = false;
	}
}

void Player::update(const float& dt)
{
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(52, this->vm));
	this->ability.setPosition(this->sprite.getPosition());
}

void Player::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
	if (this->abilityActive && (this->name == "knight" || this->name == "scout")) target.draw(this->ability);
}

void Player::drawShadow(sf::RenderTarget& target)
{
	target.draw(this->shadow);
}
