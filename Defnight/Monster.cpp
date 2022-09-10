#include "Functions.h"
#include "Monster.h"

Monster::Monster(const float& x, const float& y, sf::Texture& texture, sf::Texture& shadow_texture, const sf::VideoMode& vm, const std::string& monster_name, const float& difficulty_mod)
	:vm(vm)
{
	this->texture = texture;
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(sf::IntRect(0, 32, 16, 16));
	this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
	this->sprite.setPosition(x, y);
	this->sprite.setColor(sf::Color(255, 255, 255, 0));

	this->shadow_texture = shadow_texture;
	this->shadow.setTexture(this->shadow_texture);
	this->shadow.setScale(calcScale(4, vm), calcScale(4, vm));
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(52, vm));
	this->shadow.setColor(sf::Color(255, 255, 255, 0));

	this->name = monster_name;
	this->reach = 1;
	this->spawnCountdown = 0.f;
	this->deadCountdown = 0.f;
	this->spawned = false;

	if (this->name == "goblin") {
		this->attack = static_cast<unsigned>(1 * difficulty_mod);
		this->attackSpeed = 2;
		this->HP = static_cast<unsigned>(3 * difficulty_mod);
		this->speed = 2;
		this->gold = 1;
		this->XP = 5;
	}
	else if (this->name == "spider") {
		this->attack = static_cast<unsigned>(2 * difficulty_mod);
		this->attackSpeed = 3;
		this->HP = static_cast<unsigned>(7 * difficulty_mod);
		this->speed = 3;
		this->gold = 2;
		this->XP = 10;
	}
	else if (this->name == "orc") {
		this->attack = static_cast<unsigned>(3 * difficulty_mod);
		this->attackSpeed = 2;
		this->HP = static_cast<unsigned>(12 * difficulty_mod);
		this->speed = 2;
		this->gold = 3;
		this->XP = 17;
	}
	else if (this->name == "cyclope") {
		this->attack = static_cast<unsigned>(4 * difficulty_mod);
		this->attackSpeed = 1;
		this->HP = static_cast<unsigned>(18 * difficulty_mod);
		this->speed = 1;
		this->gold = 4;
		this->XP = 26;
	}
}

Monster::~Monster()
{
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

const bool Monster::attackPlayer(Player* player, sf::Font* font, const std::vector<sf::Sprite>& obstacles, std::list<Projectile*>& projectiles, std::list<FloatingText*>& floatingTexts)
{
	float distance = 0.f;
	if (this->getLeft())
		distance = vectorDistance(player->getCenter().x, player->getCenter().y, (this->getPosition().x + calcX(8, vm)), (this->getPosition().y + calcY(32, vm)));
	else if (this->getRight())
		distance = vectorDistance(player->getCenter().x, player->getCenter().y, (this->getPosition().x + calcX(56, vm)), (this->getPosition().y + calcY(32, vm)));
	else if (this->getUp())
		distance = vectorDistance(player->getCenter().x, player->getCenter().y, (this->getPosition().x + calcX(32, vm)), (this->getPosition().y + calcY(8, vm)));
	else if (this->getDown())
		distance = vectorDistance(player->getCenter().x, player->getCenter().y, (this->getPosition().x + calcX(32, vm)), (this->getPosition().y + calcY(56, vm)));

	if (this->name == "cyclope" && distance <= this->reach * 8.f * calcX(64, vm)) {
		if (!sightCollision(obstacles, sf::Vector2f(this->getPosition().x + calcX(24, vm), this->getPosition().y + calcY(36, vm)), player->getCenter())) {
			this->doAttack();
			if (!player->isDead() && !player->getPunched() && this->getIsAttacking() && this->getFrame() == 80) {
				if (this->getAttack() > 0) {
					projectiles.push_back(new Projectile("stone", this->getPosition().x + calcX(24, vm), this->getPosition().y + calcY(36, vm), this->getAttack(), 1, 1, sf::Vector2f(player->getPosition().x + calcX(32, vm), player->getPosition().y + calcY(32, vm)), this->vm));
					this->isAttacking = false;
				}
			}
		}
	}
	else if (distance <= this->getReach() * calcX(48, vm)) {
		this->doAttack();
		if (!player->isDead() && !player->getPunched() && this->getIsAttacking() && this->getFrame() == 80) {
			int Lattack = static_cast<int>(round(this->attack - (this->attack * player->getArmor() * 0.05f)));

			if (Lattack > 0) {
				floatingTexts.push_back(new FloatingText(font, std::to_string(-Lattack), calcChar(16, vm), player->getPosition().x + calcX(48, vm), player->getPosition().y + calcY(32, vm), sf::Color(228, 92, 95), this->vm));
				if (static_cast<int>(player->getHP() - Lattack) < 0) player->setHP(0);
				else player->setHP(player->getHP() - Lattack);
				
				player->punch();

				return true;
			}
		}
	}
	return false;
}

const bool Monster::sightCollision(const std::vector<sf::Sprite>& obstacles, const sf::Vector2f& a_p1, const sf::Vector2f& a_p2)
{
	for (auto& obs : obstacles) {
		if (vectorDistance(this->sprite.getPosition(), obs.getPosition()) <= 20.f * calcX(32, vm)) {
			if (sight(obs.getGlobalBounds(), a_p1, a_p2)) {
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

void Monster::AI(const sf::Vector2f& playerPosition, const float& dt)
{
	this->velocity = sf::Vector2f(0.f, 0.f);

	const float vel = ((this->speed * 0.2f + 0.8f) * 2 * this->sprite.getGlobalBounds().width) * dt;

	if (playerPosition.x > this->sprite.getPosition().x) 
		this->velocity.x += vel;
	if (playerPosition.x < this->sprite.getPosition().x) 
		this->velocity.x += -(vel);
	if (playerPosition.y > this->sprite.getPosition().y) 
		this->velocity.y += vel;
	if (playerPosition.y < this->sprite.getPosition().y) 
		this->velocity.y += -(vel);
	
	if (this->velocity.x != 0.f && this->velocity.y != 0.f) {
		this->velocity.x /= 1.44f;
		this->velocity.y /= 1.44f;
	}
}

void Monster::monsterCollision(const std::list<Monster*>& monsters)
{
	for (const auto& monster : monsters) {
		if (vectorDistance(this->sprite.getPosition(), monster->getPosition()) < 2 * monster->getGlobalBounds().width) {
			sf::FloatRect monsterBounds = this->sprite.getGlobalBounds();
			sf::FloatRect goblinBounds = monster->getGlobalBounds();
		
			sf::FloatRect nextPos = monsterBounds;
			nextPos.left += this->velocity.x;
			nextPos.top += this->velocity.y;
		
			if (goblinBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (monsterBounds.top < goblinBounds.top
					&& monsterBounds.top + monsterBounds.height < goblinBounds.top + goblinBounds.height
					&& monsterBounds.left < goblinBounds.left + goblinBounds.width
					&& monsterBounds.left + monsterBounds.width > goblinBounds.left)
				{
					this->velocity.y = 0.f;
					this->sprite.setPosition(monsterBounds.left, goblinBounds.top - monsterBounds.height);
				}
				//Gorna kolizja
				else if (monsterBounds.top > goblinBounds.top
					&& monsterBounds.top + monsterBounds.height > goblinBounds.top + goblinBounds.height
					&& monsterBounds.left < goblinBounds.left + goblinBounds.width
					&& monsterBounds.left + monsterBounds.width > goblinBounds.left)
				{
					this->velocity.y = 0.f;
					this->sprite.setPosition(monsterBounds.left, goblinBounds.top + goblinBounds.height);
				}
		
				//Prawa kolizja
				else if (monsterBounds.left < goblinBounds.left
					&& monsterBounds.left + monsterBounds.width < goblinBounds.left + goblinBounds.width
					&& monsterBounds.top < goblinBounds.top + goblinBounds.height
					&& monsterBounds.top + monsterBounds.height > goblinBounds.top)
				{
					this->velocity.x = 0.f;
					this->sprite.setPosition(goblinBounds.left - monsterBounds.width, monsterBounds.top);
				}
				//Lewa kolizja
				else if (monsterBounds.left > goblinBounds.left
					&& monsterBounds.left + monsterBounds.width > goblinBounds.left + goblinBounds.width
					&& monsterBounds.top < goblinBounds.top + goblinBounds.height
					&& monsterBounds.top + monsterBounds.height > goblinBounds.top)
				{
					this->velocity.x = 0.f;
					this->sprite.setPosition(goblinBounds.left + goblinBounds.width, monsterBounds.top);
				}
			}
		}
	}
}

void Monster::update(const float& dt)
{
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + calcY(52, this->vm));
}

void Monster::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}

void Monster::drawShadow(sf::RenderTarget& target)
{
	target.draw(this->shadow);
}
