#include "Functions.h"
#include "Projectile.h"

Projectile::Projectile()
{
	this->collided = false;
	this->collidedPlayer = false;
	this->collidedMob = false;
}

Projectile::~Projectile()
{
	
}

void Projectile::init(float& scale, const std::string& name, const int& attack, const unsigned& speed, const int& hp)
{
	this->scale = scale;
	this->name = name;
	this->attack = attack;
	this->speed = speed;
	this->predkosc = sf::Vector2f(0.f, 0.f);
	this->hp = hp;
}

void Projectile::calculateVelocity(const sf::Vector2f& coords, const float& dt)
{
	this->angle = getAngle(this->sprite.getPosition().x, this->sprite.getPosition().y, coords.x, coords.y) + 90.f;
	this->sprite.setRotation(this->angle);
}


const bool Projectile::sideWall(const sf::Vector2f& predkosci, const sf::FloatRect& projectileBounds, const sf::FloatRect& wallBounds, const short& side)
{
	sf::FloatRect nextPos = projectileBounds;
	nextPos.left += predkosci.x;
	nextPos.top += predkosci.y;
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

void Projectile::checkCollision(const std::vector<sf::Sprite>& obstacles)
{	
	this->collided = false;
	for (const auto& e : obstacles) {
		if (vectorDistance(this->sprite.getPosition(), e.getPosition()) < 256 * this->scale && !this->collided && !this->collidedPlayer) {

			sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
			sf::FloatRect wallBounds = e.getGlobalBounds();

			sf::FloatRect nextPos = projectileBounds;
			nextPos.left += this->predkosc.x;
			nextPos.top += this->predkosc.y;
			if (wallBounds.intersects(nextPos))
			{
				//Dolna i górna kolizja
				if ((projectileBounds.top < wallBounds.top
					&& projectileBounds.top + projectileBounds.height < wallBounds.top + wallBounds.height
					&& projectileBounds.left < wallBounds.left + wallBounds.width
					&& projectileBounds.left + projectileBounds.width > wallBounds.left) &&
					projectileBounds.top > wallBounds.top
					&& projectileBounds.top + projectileBounds.height > wallBounds.top + wallBounds.height
					&& projectileBounds.left < wallBounds.left + wallBounds.width
					&& projectileBounds.left + projectileBounds.width > wallBounds.left)
				{
					if (this->name == "Shuriken") {
						this->angle *= -1.f;
						this->collided = true;
						break;
					}
				}
				//Dolna kolizja
				else if (projectileBounds.top < wallBounds.top
					&& projectileBounds.top + projectileBounds.height < wallBounds.top + wallBounds.height
					&& projectileBounds.left < wallBounds.left + wallBounds.width
					&& projectileBounds.left + projectileBounds.width > wallBounds.left)
				{
					if (this->name == "Shuriken" && !this->sideWall(sf::Vector2f(this->predkosc.x, this->predkosc.y * -1), projectileBounds, wallBounds, 1)) {
						this->angle *= -1.f;
					}
					else {
						this->predkosc.y = 0.f;
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
					if (this->name == "Shuriken" && !this->sideWall(sf::Vector2f(this->predkosc.x, this->predkosc.y * -1), projectileBounds, wallBounds, 0)) {
						this->angle *= -1.f;
					}
					else {
						this->predkosc.y = 0.f;
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
					if (this->name == "Shuriken" && !this->sideWall(sf::Vector2f(this->predkosc.x * -1, this->predkosc.y), projectileBounds, wallBounds, 3)) {
						this->angle = 180.f - this->angle;
					}
					else {
						this->predkosc.x = 0.f;
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
					if (this->name == "Shuriken" && !this->sideWall(sf::Vector2f(this->predkosc.x * -1, this->predkosc.y), projectileBounds, wallBounds, 2)) {
						this->angle = 180.f - this->angle;
					}
					else {
						this->predkosc.x = 0.f;
						this->sprite.setPosition(wallBounds.left + wallBounds.width, projectileBounds.top);
					}
					this->collided = true;
					break;
				}

			}
		}
	}
	if (this->collided) hp--;
}

void Projectile::collisionPlayer(const sf::Sprite& player)
{
	if (vectorDistance(this->sprite.getPosition(), player.getPosition()) < 256 * this->scale && !this->collided && !this->collidedPlayer && !this->collidedMob) {

		sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
		sf::FloatRect playerBounds = player.getGlobalBounds();

		sf::FloatRect nextPos = projectileBounds;
		nextPos.left += this->predkosc.x;
		nextPos.top += this->predkosc.y;

		if (playerBounds.intersects(nextPos))
		{
			//Dolna kolizja
			if (projectileBounds.top < playerBounds.top
				&& projectileBounds.top + projectileBounds.height < playerBounds.top + playerBounds.height
				&& projectileBounds.left < playerBounds.left + playerBounds.width
				&& projectileBounds.left + projectileBounds.width > playerBounds.left)
			{
				this->predkosc.y = 0.f;
				this->sprite.setPosition(projectileBounds.left, playerBounds.top - projectileBounds.height);
				this->collidedPlayer = true;
			}
			//Gorna kolizja
			else if (projectileBounds.top > playerBounds.top
				&& projectileBounds.top + projectileBounds.height > playerBounds.top + playerBounds.height
				&& projectileBounds.left < playerBounds.left + playerBounds.width
				&& projectileBounds.left + projectileBounds.width > playerBounds.left)
			{
				this->predkosc.y = 0.f;
				this->sprite.setPosition(projectileBounds.left, playerBounds.top + playerBounds.height);
				this->collidedPlayer = true;
			}

			//Prawa kolizja
			if (projectileBounds.left < playerBounds.left
				&& projectileBounds.left + projectileBounds.width < playerBounds.left + playerBounds.width
				&& projectileBounds.top < playerBounds.top + playerBounds.height
				&& projectileBounds.top + projectileBounds.height > playerBounds.top)
			{
				this->predkosc.x = 0.f;
				this->sprite.setPosition(playerBounds.left - projectileBounds.width, projectileBounds.top);
				this->collidedPlayer = true;
			}
			//Lewa kolizja
			else if (projectileBounds.left > playerBounds.left
				&& projectileBounds.left + projectileBounds.width > playerBounds.left + playerBounds.width
				&& projectileBounds.top < playerBounds.top + playerBounds.height
				&& projectileBounds.top + projectileBounds.height > playerBounds.top)
			{
				this->predkosc.x = 0.f;
				this->sprite.setPosition(playerBounds.left + playerBounds.width, projectileBounds.top);
				this->collidedPlayer = true;
			}

		}
	}
}

void Projectile::collisionMob(const sf::Sprite& mob)
{
	if (vectorDistance(this->sprite.getPosition(), mob.getPosition()) < 256 * this->scale && !this->collided && !this->collidedPlayer && !this->collidedMob) {

		sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
		sf::FloatRect mobBounds = mob.getGlobalBounds();

		sf::FloatRect nextPos = projectileBounds;
		nextPos.left += this->predkosc.x;
		nextPos.top += this->predkosc.y;

		if (mobBounds.intersects(nextPos))
		{
			//Dolna kolizja
			if (projectileBounds.top < mobBounds.top
				&& projectileBounds.top + projectileBounds.height < mobBounds.top + mobBounds.height
				&& projectileBounds.left < mobBounds.left + mobBounds.width
				&& projectileBounds.left + projectileBounds.width > mobBounds.left)
			{
				this->predkosc.y = 0.f;
				this->sprite.setPosition(projectileBounds.left, mobBounds.top - projectileBounds.height);
				this->collidedMob = true;
			}
			//Gorna kolizja
			else if (projectileBounds.top > mobBounds.top
				&& projectileBounds.top + projectileBounds.height > mobBounds.top + mobBounds.height
				&& projectileBounds.left < mobBounds.left + mobBounds.width
				&& projectileBounds.left + projectileBounds.width > mobBounds.left)
			{
				this->predkosc.y = 0.f;
				this->sprite.setPosition(projectileBounds.left, mobBounds.top + mobBounds.height);
				this->collidedMob = true;
			}

			//Prawa kolizja
			if (projectileBounds.left < mobBounds.left
				&& projectileBounds.left + projectileBounds.width < mobBounds.left + mobBounds.width
				&& projectileBounds.top < mobBounds.top + mobBounds.height
				&& projectileBounds.top + projectileBounds.height > mobBounds.top)
			{
				this->predkosc.x = 0.f;
				this->sprite.setPosition(mobBounds.left - projectileBounds.width, projectileBounds.top);
				this->collidedMob = true;
			}
			//Lewa kolizja
			else if (projectileBounds.left > mobBounds.left
				&& projectileBounds.left + projectileBounds.width > mobBounds.left + mobBounds.width
				&& projectileBounds.top < mobBounds.top + mobBounds.height
				&& projectileBounds.top + projectileBounds.height > mobBounds.top)
			{
				this->predkosc.x = 0.f;
				this->sprite.setPosition(mobBounds.left + mobBounds.width, projectileBounds.top);
				this->collidedMob = true;
			}

		}
	}
}

void Projectile::move(const float& dt)
{
	this->predkosc.x = (this->speed * 0.2f + 0.8f) * 256.f * dt * this->scale * cos((3.1415f / 180) * this->angle);
	this->predkosc.y = (this->speed * 0.2f + 0.8f) * 256.f * dt * this->scale * sin((3.1415f / 180) * this->angle);
	this->sprite.move(this->predkosc);
}
