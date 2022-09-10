#include "Functions.h"
#include "Projectile.h"

Projectile::Projectile(const std::string& name, const float& posX, const float& posY,
	const unsigned& attack, const unsigned& speed, const unsigned& hp, const sf::Vector2f& coords, sf::VideoMode& vm)
	:name(name), vm(vm), attack(attack), speed(speed), HP(hp)
{
	this->texture.loadFromFile("external/assets/projectiles.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
	this->sprite.setPosition(posX, posY);
	this->sprite.setOrigin(2.f, 2.f);

	this->velocity = sf::Vector2f(0.f, 0.f);
	this->angle = 0.f;
	this->collided = false;
	this->collidedPlayer = false;
	this->collidedMonster = false;

	if (this->name == "stone") this->sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
	else this->sprite.setTextureRect(sf::IntRect(4, 0, 4, 4));

	this->calculateVelocity(coords);
}

Projectile::~Projectile()
{
	
}

const sf::Vector2f& Projectile::getPosition() const
{
	return this->sprite.getPosition();
}

const unsigned Projectile::getHP() const
{
	return this->HP;
}

const bool Projectile::getCollided() const
{
	return this->collided;
}

const bool Projectile::getCollidedPlayer() const
{
	return this->collidedPlayer;
}

const bool Projectile::getCollidedMonster() const
{
	return this->collidedMonster;
}

const unsigned Projectile::getAttack() const
{
	return this->attack;
}

void Projectile::calculateVelocity(const sf::Vector2f& coords)
{
	this->angle = getAngle(this->sprite.getPosition().x, this->sprite.getPosition().y, coords.x, coords.y) + 90.f;
	this->sprite.setRotation(this->angle);
}

const bool Projectile::sideWall(const sf::Vector2f& velocities, const sf::FloatRect& projectileBounds, const sf::FloatRect& wallBounds, const short& side)
{
	sf::FloatRect nextPos = projectileBounds;
	nextPos.left += velocities.x;
	nextPos.top += velocities.y;
	if (wallBounds.intersects(nextPos))
	{
		if (side == 0) {
			if (projectileBounds.top < wallBounds.top
				&& projectileBounds.top + projectileBounds.height < wallBounds.top + wallBounds.height
				&& projectileBounds.left < wallBounds.left + wallBounds.width
				&& projectileBounds.left + projectileBounds.width > wallBounds.left) {
				return true;	
			}		
		}
		else if (side == 1) {
			if (projectileBounds.top > wallBounds.top
				&& projectileBounds.top + projectileBounds.height > wallBounds.top + wallBounds.height
				&& projectileBounds.left < wallBounds.left + wallBounds.width
				&& projectileBounds.left + projectileBounds.width > wallBounds.left) {
				return true;	
			}
		}
		else if (side == 2) {
			if (projectileBounds.left < wallBounds.left
				&& projectileBounds.left + projectileBounds.width < wallBounds.left + wallBounds.width
				&& projectileBounds.top < wallBounds.top + wallBounds.height
				&& projectileBounds.top + projectileBounds.height > wallBounds.top) {
				return true;	
			}
		}
		else if (side == 3) {
			if (projectileBounds.left > wallBounds.left
				&& projectileBounds.left + projectileBounds.width > wallBounds.left + wallBounds.width
				&& projectileBounds.top < wallBounds.top + wallBounds.height
				&& projectileBounds.top + projectileBounds.height > wallBounds.top) {
				return true;	
			}
		}
	}
		
	return false;
}

void Projectile::obstacleCollision(const std::vector<sf::Sprite>& obstacles)
{	
	this->collided = false;

	const float distance = 2 * obstacles.front().getGlobalBounds().width;

	for (const auto& e : obstacles) {
		if (vectorDistance(this->sprite.getPosition(), e.getPosition()) < distance && !this->collided && !this->collidedPlayer) {

			sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
			sf::FloatRect wallBounds = e.getGlobalBounds();

			sf::FloatRect nextPos = projectileBounds;
			nextPos.left += this->velocity.x;
			nextPos.top += this->velocity.y;
			if (wallBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (projectileBounds.top < wallBounds.top
					&& projectileBounds.top + projectileBounds.height < wallBounds.top + wallBounds.height
					&& projectileBounds.left < wallBounds.left + wallBounds.width
					&& projectileBounds.left + projectileBounds.width > wallBounds.left)
				{
					if (this->name == "shuriken" && !this->sideWall(sf::Vector2f(this->velocity.x, this->velocity.y * -1), projectileBounds, wallBounds, 1)) {
						this->angle *= -1.f;
					}
					else {
						this->velocity.y = 0.f;
						this->sprite.setPosition(projectileBounds.left, wallBounds.top - projectileBounds.height);
					}
					this->collided = true;
					break;
				}
				//Gorna kolizja
				else if (projectileBounds.top > wallBounds.top
					&& projectileBounds.top + projectileBounds.height > wallBounds.top + wallBounds.height
					&& projectileBounds.left < wallBounds.left + wallBounds.width
					&& projectileBounds.left + projectileBounds.width > wallBounds.left)
				{
					if (this->name == "shuriken" && !this->sideWall(sf::Vector2f(this->velocity.x, this->velocity.y * -1), projectileBounds, wallBounds, 0)) {
						this->angle *= -1.f;
					}
					else {
						this->velocity.y = 0.f;
						this->sprite.setPosition(projectileBounds.left, wallBounds.top + wallBounds.height);
					}
					this->collided = true;
					break;
				}

				//Prawa kolizja
				if (projectileBounds.left < wallBounds.left
					&& projectileBounds.left + projectileBounds.width < wallBounds.left + wallBounds.width
					&& projectileBounds.top < wallBounds.top + wallBounds.height
					&& projectileBounds.top + projectileBounds.height > wallBounds.top)
				{
					if (this->name == "shuriken" && !this->sideWall(sf::Vector2f(this->velocity.x * -1, this->velocity.y), projectileBounds, wallBounds, 3)) {
						this->angle = 180.f - this->angle;
					}
					else {
						this->velocity.x = 0.f;
						this->sprite.setPosition(wallBounds.left - projectileBounds.width, projectileBounds.top);
					}
					this->collided = true;
					break;
				}
				//Lewa kolizja
				else if (projectileBounds.left > wallBounds.left
					&& projectileBounds.left + projectileBounds.width > wallBounds.left + wallBounds.width
					&& projectileBounds.top < wallBounds.top + wallBounds.height
					&& projectileBounds.top + projectileBounds.height > wallBounds.top)
				{
					if (this->name == "shuriken" && !this->sideWall(sf::Vector2f(this->velocity.x * -1, this->velocity.y), projectileBounds, wallBounds, 2)) {
						this->angle = 180.f - this->angle;
					}
					else {
						this->velocity.x = 0.f;
						this->sprite.setPosition(wallBounds.left + wallBounds.width, projectileBounds.top);
					}
					this->collided = true;
					break;
				}

			}
		}
	}
	if (this->collided) this->HP--;
}

void Projectile::playerCollision(Player* player)
{
	if (this->name == "stone") {

		const float distance = 2 * player->getGlobalBounds().width;

		if (vectorDistance(this->sprite.getPosition(), player->getPosition()) < distance && !this->collided && !this->collidedPlayer && !this->collidedMonster) {

			sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
			sf::FloatRect playerBounds = player->getGlobalBounds();

			sf::FloatRect nextPos = projectileBounds;
			nextPos.left += this->velocity.x;
			nextPos.top += this->velocity.y;

			if (playerBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (projectileBounds.top < playerBounds.top
					&& projectileBounds.top + projectileBounds.height < playerBounds.top + playerBounds.height
					&& projectileBounds.left < playerBounds.left + playerBounds.width
					&& projectileBounds.left + projectileBounds.width > playerBounds.left)
				{
					this->velocity.y = 0.f;
					this->sprite.setPosition(projectileBounds.left, playerBounds.top - projectileBounds.height);
					this->collidedPlayer = true;
				}
				//Gorna kolizja
				else if (projectileBounds.top > playerBounds.top
					&& projectileBounds.top + projectileBounds.height > playerBounds.top + playerBounds.height
					&& projectileBounds.left < playerBounds.left + playerBounds.width
					&& projectileBounds.left + projectileBounds.width > playerBounds.left)
				{
					this->velocity.y = 0.f;
					this->sprite.setPosition(projectileBounds.left, playerBounds.top + playerBounds.height);
					this->collidedPlayer = true;
				}

				//Prawa kolizja
				if (projectileBounds.left < playerBounds.left
					&& projectileBounds.left + projectileBounds.width < playerBounds.left + playerBounds.width
					&& projectileBounds.top < playerBounds.top + playerBounds.height
					&& projectileBounds.top + projectileBounds.height > playerBounds.top)
				{
					this->velocity.x = 0.f;
					this->sprite.setPosition(playerBounds.left - projectileBounds.width, projectileBounds.top);
					this->collidedPlayer = true;
				}
				//Lewa kolizja
				else if (projectileBounds.left > playerBounds.left
					&& projectileBounds.left + projectileBounds.width > playerBounds.left + playerBounds.width
					&& projectileBounds.top < playerBounds.top + playerBounds.height
					&& projectileBounds.top + projectileBounds.height > playerBounds.top)
				{
					this->velocity.x = 0.f;
					this->sprite.setPosition(playerBounds.left + playerBounds.width, projectileBounds.top);
					this->collidedPlayer = true;
				}

			}
		}
	}
}

void Projectile::monsterCollision(Monster* monster, sf::Font* font, Player* player, std::list<FloatingText*>& floatingTexts)
{
	if (this->name == "shuriken") {
		const float distance = 2 * monster->getGlobalBounds().width;

		if (vectorDistance(this->sprite.getPosition(), monster->getPosition()) < distance && !this->collided && !this->collidedPlayer && !this->collidedMonster) {

			sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
			sf::FloatRect mobBounds = monster->getGlobalBounds();

			sf::FloatRect nextPos = projectileBounds;
			nextPos.left += this->velocity.x;
			nextPos.top += this->velocity.y;

			if (mobBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (projectileBounds.top < mobBounds.top
					&& projectileBounds.top + projectileBounds.height < mobBounds.top + mobBounds.height
					&& projectileBounds.left < mobBounds.left + mobBounds.width
					&& projectileBounds.left + projectileBounds.width > mobBounds.left)
				{
					this->velocity.y = 0.f;
					this->sprite.setPosition(projectileBounds.left, mobBounds.top - projectileBounds.height);
					this->collidedMonster = true;
				}
				//Gorna kolizja
				else if (projectileBounds.top > mobBounds.top
					&& projectileBounds.top + projectileBounds.height > mobBounds.top + mobBounds.height
					&& projectileBounds.left < mobBounds.left + mobBounds.width
					&& projectileBounds.left + projectileBounds.width > mobBounds.left)
				{
					this->velocity.y = 0.f;
					this->sprite.setPosition(projectileBounds.left, mobBounds.top + mobBounds.height);
					this->collidedMonster = true;
				}

				//Prawa kolizja
				if (projectileBounds.left < mobBounds.left
					&& projectileBounds.left + projectileBounds.width < mobBounds.left + mobBounds.width
					&& projectileBounds.top < mobBounds.top + mobBounds.height
					&& projectileBounds.top + projectileBounds.height > mobBounds.top)
				{
					this->velocity.x = 0.f;
					this->sprite.setPosition(mobBounds.left - projectileBounds.width, projectileBounds.top);
					this->collidedMonster = true;
				}
				//Lewa kolizja
				else if (projectileBounds.left > mobBounds.left
					&& projectileBounds.left + projectileBounds.width > mobBounds.left + mobBounds.width
					&& projectileBounds.top < mobBounds.top + mobBounds.height
					&& projectileBounds.top + projectileBounds.height > mobBounds.top)
				{
					this->velocity.x = 0.f;
					this->sprite.setPosition(mobBounds.left + mobBounds.width, projectileBounds.top);
					this->collidedMonster = true;
				}

			}

			if (this->collidedMonster) {
				if ((unsigned(Random::Float() * 100.f) + 1) <= player->getCriticalChance()) {
					const int attack = 2 * this->getAttack();
					floatingTexts.push_back(new FloatingText(font, std::to_string(-attack), calcChar(16, vm), monster->getPosition().x + calcX(32, vm), monster->getPosition().y + calcY(32, vm), sf::Color(233, 134, 39), this->vm));
					if (static_cast<int>(monster->getHP() - attack) < 0) monster->setHP(0);
					else monster->setHP(monster->getHP() - attack);
				}
				else {
					const int attack = this->getAttack();
					floatingTexts.push_back(new FloatingText(font, std::to_string(-attack), calcChar(16, vm), monster->getPosition().x + calcX(32, vm), monster->getPosition().y + calcY(32, vm), sf::Color(255, 255, 255), this->vm));
					if (static_cast<int>(monster->getHP() - attack) < 0) monster->setHP(0);
					else monster->setHP(monster->getHP() - attack);
				}

				monster->punch();

				this->HP = 0;
			}
		}
	}
}

void Projectile::move(const float& dt)
{
	const float vel = (this->speed * 0.2f + 0.8f) * 16.f * this->sprite.getGlobalBounds().width * dt;

	this->velocity.x = vel * cos((3.1415f / 180.f) * this->angle);
	this->velocity.y = vel * sin((3.1415f / 180.f) * this->angle);
	this->sprite.move(this->velocity);
}

void Projectile::update(const float& dt)
{
	if (this->name == "shuriken") this->sprite.rotate(90.f / dt);
	this->move(dt);
}

void Projectile::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
