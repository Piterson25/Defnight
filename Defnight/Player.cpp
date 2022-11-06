#include "Functions.h"
#include "Player.h"

Player::Player(const float& x, const float& y, const sf::VideoMode& vm, const float& soundVolume, const std::string& hero_name)
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

	this->whoosh.loadFromFile("external/music/swipe.wav");
	this->hit.loadFromFile("external/music/hit.wav");
	this->sound.setBuffer(this->whoosh);
	this->sound.setVolume(soundVolume);
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
		this->attack = 3;
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

const uint16_t Player::getGold() const
{
	return this->gold;
}

const uint16_t Player::getAttack() const
{
	return this->attack;
}

const uint16_t Player::getKills() const
{
	return this->kills;
}

const bool Player::getSpawned() const
{
	return this->spawned;
}

const uint16_t Player::getArmor() const
{
	return this->armor;
}

const unsigned Player::getReg() const
{
	return this->reg;
}

const unsigned Player::getMaxXP() const
{
	return this->maxXP;
}

const unsigned Player::getLastMaxXP() const
{
	return this->lastMaxXP;
}

const unsigned Player::getLevel() const
{
	return this->level;
}

const unsigned Player::getCriticalChance() const
{
	return this->criticalChance;
}

const bool Player::getRegenerating() const
{
	return this->isRegenerating;
}

const bool Player::getLeveling() const
{
	return this->isLeveling;
}

const float Player::getAbilityCooldown() const
{
	return this->abilityCooldown;
}

const float Player::getAbilityMaxTime() const
{
	return this->abilityMaxTime;
}

const float Player::getAbilityTime() const
{
	return this->abilityTime;
}

const bool Player::getAbilityActive() const
{
	return this->abilityActive;
}

void Player::setGold(const uint16_t& gold)
{
	this->gold = gold;
}

void Player::setAttack(const uint16_t& attack)
{
	this->attack = attack;
}

void Player::setKills(const uint16_t& kills)
{
	this->kills = kills;
}

void Player::setCriticalChance(const unsigned& criticalChance)
{
	this->criticalChance = criticalChance;
}

void Player::setReg(const unsigned& reg)
{
	this->reg = reg;
}

void Player::setArmor(const uint16_t& armor)
{
	this->armor = armor;
}

void Player::setIsRegenerating(const bool& isRegenerating)
{
	this->isRegenerating = isRegenerating;
}

void Player::setIsLeveling(const bool& isLeveling)
{
	this->isLeveling = isLeveling;
}

void Player::setAbilityMaxTime(const float& abilityMaxTime)
{
	this->abilityMaxTime = abilityMaxTime;
}

void Player::setAbilityTime(const float& abilityTime)
{
	this->abilityTime = abilityTime;
}

void Player::setAbilityCooldown(const float& abilityCooldown)
{
	this->abilityCooldown = abilityCooldown;
}

void Player::attackMonster(sf::Font* font, const std::list<Monster*>& monsters, std::list<FloatingText*>& floatingTexts)
{
	const sf::VideoMode vm = this->vm;
	for (const auto& monster : monsters) {
		const float distance = this->attackDistance(monster, this);

		if ((hasVelocity() && distance <= this->getReach() * calcX(32, vm)) || (!hasVelocity() && distance <= this->getReach() * calcX(48, vm))) {

			if (!monster->isDead() && !monster->getPunched() && monster->getSpawned() && this->getIsAttacking() && this->getFrame() == 80) {
				if ((unsigned(Random::Float() * 100.f) + 1) <= this->getCriticalChance()) {
					const int attack = 2 * this->getAttack();
					floatingTexts.push_back(new FloatingText(font, std::to_string(-attack), calcChar(16, vm), monster->getPosition().x + calcX(32, vm), monster->getPosition().y + calcY(32, vm), sf::Color(233, 134, 39), false, this->vm));
					if (static_cast<int>(monster->getHP() - attack) < 0) monster->setHP(0);
					else monster->setHP(monster->getHP() - attack);
				}
				else {
					const int attack = this->getAttack();
					floatingTexts.push_back(new FloatingText(font, std::to_string(-attack), calcChar(16, vm), monster->getPosition().x + calcX(32, vm), monster->getPosition().y + calcY(32, vm), sf::Color(255, 255, 255), false, this->vm));
					if (static_cast<int>(monster->getHP() - attack) < 0) monster->setHP(0);
					else monster->setHP(monster->getHP() - attack);
				}

				if (this->sound.getStatus() == sf::Sound::Stopped && !this->playedSound) {
					this->sound.setBuffer(this->hit);
					this->sound.play();
					this->playedSound = true;
				}
				
				monster->punch();
				break;
			}

		}
	}
}

const bool Player::addXP(const unsigned& monsterXP)
{
	this->setIsLeveling(true);
	this->setXP(this->getXP() + monsterXP);

	bool addedlevel = false;

	while (this->XP >= this->maxXP) {
		this->level++;
		this->lastMaxXP = this->maxXP;
		this->maxXP += (static_cast<unsigned>(Random::Float() * this->maxXP + this->maxXP));
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
		this->velocity *= 1.5f;
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

void Player::doAbility(const sf::Vector2f& coords, std::list<Projectile*>& projectiles)
{
	if (this->name == "ninja") {
		projectiles.push_back(new Projectile("shuriken", this->getPosition().x + calcX(32, vm), this->getPosition().y + calcY(32, vm), 3, 4, 3, coords, this->vm));
	}
	else if (this->name == "knight") {
		this->armor += 5;
	}
}

void Player::update(const float& dt)
{
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(52, this->vm));
	this->ability.setPosition(this->sprite.getPosition());

	if (this->isAttacking && this->frame == 80 && this->sound.getStatus() == sf::Sound::Stopped && !this->playedSound) {
		this->sound.setBuffer(this->whoosh);
		this->sound.play();
		this->playedSound = true;
	}
	if (this->sound.getStatus() == sf::Sound::Stopped && this->playedSound && this->frame != 80) {
		this->playedSound = false;
	}
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
