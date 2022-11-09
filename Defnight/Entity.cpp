#include "stdafx.h"
#include "Functions.h"
#include "Entity.h"

Entity::Entity()
{
	this->vm = sf::VideoMode();
	this->velocity = sf::Vector2f(0.f, 0.f);
	this->name = "";
	this->attackSpeed = 0;
	this->maxHP = 0;
	this->HP = 0;
	this->XP = 0;
	this->speed = 0;
	this->reach = 0;

	this->up = false;
	this->left = true;
	this->down = false;
	this->right = false;

	this->animationCooldown = 0.f;
	this->frame = 0;

	this->attackCooldown = 0.f;
	this->isAttacking = false;
	this->punchedCooldown = 0.f;
	this->punched = false;
}

Entity::~Entity()
{

}

const sf::Vector2f& Entity::getPosition() const
{
	return this->sprite.getPosition();
}

const sf::Texture& Entity::getTexture() const
{
	return this->texture;
}

const sf::Vector2f& Entity::getVelocity() const
{
	return this->velocity;
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

const bool Entity::isDead() const
{
	if (this->HP == 0) return true;
	return false;
}

const bool Entity::hasVelocity() const
{
	if (this->velocity != sf::Vector2f(0.f, 0.f)) return true;
	else return false;
}

const bool Entity::getLeft() const
{
	return this->left;
}

const bool Entity::getUp() const
{
	return this->up;
}

const bool Entity::getRight() const
{
	return this->right;
}

const bool Entity::getDown() const
{
	return this->down;
}

const bool Entity::getIsAttacking() const
{
	return this->isAttacking;
}

const unsigned Entity::getAttackSpeed() const
{
	return this->attackSpeed;
}

const unsigned Entity::getMaxHP() const
{
	return this->maxHP;
}

const unsigned Entity::getHP() const
{
	return this->HP;
}

const unsigned Entity::getXP() const
{
	return this->XP;
}

const unsigned Entity::getSpeed() const
{
	return this->speed;
}

const unsigned Entity::getReach() const
{
	return this->reach;
}

const unsigned Entity::getPunched() const
{
	return this->punched;
}

const unsigned Entity::getFrame() const
{
	return this->frame;
}

const sf::Vector2f Entity::getCenter() const
{
	return sf::Vector2f(this->sprite.getPosition().x + (this->sprite.getGlobalBounds().width * 0.5f), this->sprite.getPosition().y + (this->sprite.getGlobalBounds().height * 0.5f));
}

const sf::Vector2f Entity::getLeftCenter() const
{
	return sf::Vector2f(this->sprite.getPosition().x + (this->sprite.getGlobalBounds().width * 0.125f), this->sprite.getPosition().y + (this->sprite.getGlobalBounds().height * 0.5f));
}

const sf::Vector2f Entity::getUpCenter() const
{
	return sf::Vector2f(this->sprite.getPosition().x + (this->sprite.getGlobalBounds().width * 0.5f), this->sprite.getPosition().y + (this->sprite.getGlobalBounds().height * 0.125f));
}

const sf::Vector2f Entity::getDownCenter() const
{
	return sf::Vector2f(this->sprite.getPosition().x + (this->sprite.getGlobalBounds().width * 0.5f), this->sprite.getPosition().y + (this->sprite.getGlobalBounds().height * 0.875f));
}

const sf::Vector2f Entity::getRightCenter() const
{
	return sf::Vector2f(this->sprite.getPosition().x + (this->sprite.getGlobalBounds().width * 0.875f), this->sprite.getPosition().y + (this->sprite.getGlobalBounds().height * 0.5f));
}

const float Entity::attackDistance(Entity* e1, Entity* e2) const
{
	if (this->left)
		return vectorDistance(e1->getCenter(), e2->getLeftCenter());
	else if (this->right)
		return vectorDistance(e1->getCenter(), e2->getRightCenter());
	else if (this->up)
		return vectorDistance(e1->getCenter(), e2->getUpCenter());
	else if (this->down)
		return vectorDistance(e1->getCenter(), e2->getDownCenter());
	return 1000.f;
}

const std::string& Entity::getName() const
{
	return this->name;
}

void Entity::setVeloctiy(const sf::Vector2f& velocity)
{
	this->velocity = velocity;
}

void Entity::setTexturePath(const std::string& path)
{
	this->texture.loadFromFile(path);
	this->sprite.setTexture(this->texture);
}

void Entity::setTexture(const sf::Texture& texture)
{
	this->sprite.setTexture(texture);
}

void Entity::setPosition(const float& x, const float& y)
{
	this->sprite.setPosition(x, y);
}


void Entity::setAttackSpeed(const unsigned& attackSpeed)
{
	this->attackSpeed = attackSpeed;
}

void Entity::setMaxHP(const unsigned& maxHP)
{
	this->maxHP = maxHP;
}

void Entity::setHP(const unsigned& HP)
{
	this->HP = HP;
}

void Entity::setXP(const unsigned& XP)
{
	this->XP = XP;
}

void Entity::setSpeed(const unsigned& speed)
{
	this->speed = speed;
}

void Entity::setReach(const unsigned& reach)
{
	this->reach = reach;
}

void Entity::setName(const std::string& name)
{
	this->name = name;
}

void Entity::loadAttack(const float& dt)
{
	if (this->attackCooldown < 1.f)
		this->attackCooldown += this->attackSpeed * dt;
}

void Entity::doAttack()
{
	if (!this->isAttacking && this->attackCooldown >= 1.f) {
		this->isAttacking = true;
		this->attackCooldown = 0.f;
		this->frame = 0;
	}
}

void Entity::obstacleCollision(const std::vector<Tile*>& tiles)
{
	const float distance = 2 * tiles.front()->getGlobalBounds().width;

	for (const auto& e : tiles) {
		if (vectorDistance(this->sprite.getPosition(), e->getPosition()) < distance) {

			sf::FloatRect playerBounds = this->sprite.getGlobalBounds();
			sf::FloatRect wallBounds = e->getGlobalBounds();

			sf::FloatRect nextPos = playerBounds;
			nextPos.left += this->velocity.x;
			nextPos.top += this->velocity.y;

			if (wallBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (playerBounds.top < wallBounds.top
					&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
					&& playerBounds.left < wallBounds.left + wallBounds.width
					&& playerBounds.left + playerBounds.width > wallBounds.left)
				{
					this->velocity.y = 0.f;
					this->sprite.setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
				}
				//Gorna kolizja
				else if (playerBounds.top > wallBounds.top
					&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
					&& playerBounds.left < wallBounds.left + wallBounds.width
					&& playerBounds.left + playerBounds.width > wallBounds.left)
				{
					this->velocity.y = 0.f;
					this->sprite.setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
				}

				//Prawa kolizja
				if (playerBounds.left < wallBounds.left
					&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
					&& playerBounds.top < wallBounds.top + wallBounds.height
					&& playerBounds.top + playerBounds.height > wallBounds.top)
				{
					this->velocity.x = 0.f;
					this->sprite.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
				}
				//Lewa kolizja
				else if (playerBounds.left > wallBounds.left
					&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
					&& playerBounds.top < wallBounds.top + wallBounds.height
					&& playerBounds.top + playerBounds.height > wallBounds.top)
				{
					this->velocity.x = 0.f;
					this->sprite.setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
				}

			}
		}

	}
}

void Entity::animation(const float& dt)
{
	if (this->velocity.x < 0) {
		this->left = true;
		this->right = false;
		this->up = false;
		this->down = false;
	}
	else if (this->velocity.x > 0) {
		this->left = false;
		this->right = true;
		this->up = false;
		this->down = false;
	}
	else {
		if (this->velocity.y < 0) {
			this->up = true;
			this->down = false;
			this->left = false;
			this->right = false;
		}
		else if (this->velocity.y > 0) {
			this->up = false;
			this->down = true;
			this->left = false;
			this->right = false;
		}
	}

	uint8_t monsterTile = 1;
	if (this->name == "minotaur") monsterTile = 2;
		
	if (this->isAttacking && (this->velocity.x != 0.f || this->velocity.y != 0.f)) {
		
		int y = 0;
		if (this->left) y = 10;
		else if (this->right) y = 11;
		else {
			if (this->up) y = 8;
			else if (this->down) y = 9;
		}
		
		this->animationCooldown += (this->attackSpeed * 0.2f + 0.8f) * 8.f * dt;
		if (this->animationCooldown >= 1.f) {
			this->animationCooldown = 0.f;
		
			sf::IntRect intRect(this->frame, y * 16 * monsterTile, 16 * monsterTile, 16 * monsterTile);
			this->sprite.setTextureRect(intRect);
		
			if (this->frame == 112 * monsterTile) {
				this->frame = 0;
				this->isAttacking = false;
			}
			else this->frame += 16 * monsterTile;
		}
	}
	else if (this->isAttacking) {
				
		int y = 0;
		if (this->left) y = 6;
		else if (this->right) y = 7;
		else {
			if (this->up) y = 4;
			else if (this->down) y = 5;
		}
		
		this->animationCooldown += (this->attackSpeed * 0.2f + 0.8f) * 8.f * dt;
		if (this->animationCooldown >= 1.f) {
			this->animationCooldown = 0.f;
		
			sf::IntRect intRect(this->frame, y * 16 * monsterTile, 16 * monsterTile, 16 * monsterTile);
			this->sprite.setTextureRect(intRect);
		
			if (this->frame == 112 * monsterTile) {
				this->frame = 0;
				this->isAttacking = false;
			}
			else this->frame += 16 * monsterTile;
		}
	}
	else if (this->velocity.x != 0.f || this->velocity.y != 0.f) {
		int y = 0;
		if (this->velocity.x < 0) y = 2;
		else if (this->velocity.x > 0) y = 3;
		else {
			if (this->velocity.y < 0) y = 0;
			else if (this->velocity.y > 0) y = 1;
		}
		
		this->animationCooldown += (this->speed * 0.2f + 0.8f) * 4.f * dt;
		if (this->animationCooldown >= 1.f) {
			this->animationCooldown = 0.f;
		
			sf::IntRect intRect(this->frame, y * 16 * monsterTile, 16 * monsterTile, 16 * monsterTile);
			this->sprite.setTextureRect(intRect);
		
			if (this->frame == 112 * monsterTile) this->frame = 0;
			else this->frame += 16 * monsterTile;
		}
	}
}

void Entity::punch()
{
	this->punched = true;
	this->punchedCooldown = 0.f;
	this->frame = 0;
	this->isAttacking = false;
	if (this->HP == 0) {
		this->sprite.setColor(sf::Color(182, 60, 53));
	}
}

void Entity::smashed(const float& dt)
{
	if (this->punched && !this->isDead()) {
		if (this->punchedCooldown < 1.f) {
			this->punchedCooldown += 5.f * dt;
			this->sprite.setColor(sf::Color(182, 60, 53));
		}
		if (this->punchedCooldown >= 1.f) {
			this->punchedCooldown = 0.f;
			this->sprite.setColor(sf::Color::White);
			this->punched = false;
		}
	}
}

void Entity::move()
{
	this->sprite.move(this->velocity);
}
