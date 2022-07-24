#include "Functions.h"
#include "Player.h"


Player::Player()
{
	
}

Player::~Player()
{

}

void Player::init(float& scale, const std::string& nazwa)
{
	this->name = nazwa;
	this->animationTime = 0.f;
	this->step = 0;
	this->attack = 3;
	this->attackCooldown = 0;
	this->attackSpeed = 3;
	this->regCooldown = 0;
	this->Level = 1;
	this->Reg = 1;
	this->maxhp = 10;
	this->hp = 10;
	this->maxxp = 40;
	this->xp = 0;
	this->speed = 4;
	this->critical = 10;
	this->reach = 1;
	this->gold = 0;
	this->predkosc = sf::Vector2f(0.f, 0.f);
	this->punched = false;
	this->punchedTime = 0.f;
	this->attackAni = false;
	this->up = false;
	this->down = false;
	this->left = true;
	this->right = false;
	this->dead = false;
	this->abilityCounter = 0.f;
	this->abilityCooldown = 0.f;
	this->abilityDuration = 0.f;
	this->abilityActive = false;

	this->scale = scale;
	this->sprite.setTexture(this->texture);
	sf::IntRect intRect(0, 32, 16, 16);
	this->sprite.setTextureRect(intRect);
	this->sprite.setScale(4 * this->scale, 4 * this->scale);
	this->sprite.setColor(sf::Color::White);

}

void Player::controls(std::vector<sf::Keyboard::Key>& klawisze, const float& dt)
{
	this->predkosc = sf::Vector2f(0.f, 0.f);

	const float war = (this->speed * 0.2f + 0.8f) * 128.f * dt * this->scale;

	if (sf::Keyboard::isKeyPressed(klawisze[0]))
		this->predkosc.y += -(war);
	if (sf::Keyboard::isKeyPressed(klawisze[1]))
		this->predkosc.y += war;
	if (sf::Keyboard::isKeyPressed(klawisze[2]))
		this->predkosc.x += -(war);
	if (sf::Keyboard::isKeyPressed(klawisze[3]))
		this->predkosc.x += war;
}

void Player::collision(const std::vector<sf::Sprite>& obstacles)
{
	for (const auto& e : obstacles) {
		if (vectorDistance(this->sprite.getPosition(), e.getPosition()) < 256 * this->scale) {

			sf::FloatRect playerBounds = this->sprite.getGlobalBounds();
			sf::FloatRect wallBounds = e.getGlobalBounds();

			sf::FloatRect nextPos = playerBounds;
			nextPos.left += this->predkosc.x;
			nextPos.top += this->predkosc.y;

			if (wallBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (playerBounds.top < wallBounds.top
					&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
					&& playerBounds.left < wallBounds.left + wallBounds.width
					&& playerBounds.left + playerBounds.width > wallBounds.left)
				{
					this->predkosc.y = 0.f;
					this->sprite.setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
				}
				//Gorna kolizja
				else if (playerBounds.top > wallBounds.top
					&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
					&& playerBounds.left < wallBounds.left + wallBounds.width
					&& playerBounds.left + playerBounds.width > wallBounds.left)
				{
					this->predkosc.y = 0.f;
					this->sprite.setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
				}

				//Prawa kolizja
				if (playerBounds.left < wallBounds.left
					&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
					&& playerBounds.top < wallBounds.top + wallBounds.height
					&& playerBounds.top + playerBounds.height > wallBounds.top)
				{
					this->predkosc.x = 0.f;
					this->sprite.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
				}
				//Lewa kolizja
				else if (playerBounds.left > wallBounds.left
					&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
					&& playerBounds.top < wallBounds.top + wallBounds.height
					&& playerBounds.top + playerBounds.height > wallBounds.top)
				{
					this->predkosc.x = 0.f;
					this->sprite.setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
				}

			}
		}

	}
}

void Player::damaged(const float& dt)
{
	if (this->punched) {
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

void Player::animation(const float& dt)
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

void Player::move()
{
	if (this->name == "Skaut" && this->abilityActive) {
		this->predkosc.x *= 1.5f;
		this->predkosc.y *= 1.5f;
	}
	this->sprite.move(this->predkosc);
	this->shadow.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + 52 * this->scale);
}

bool Player::regeneration(const float& dt)
{
	if (this->regCooldown < 1.f && this->hp < this->maxhp)
		this->regCooldown += (this->Reg * 0.4f + 0.8f) / 4.f * dt;
	if (this->regCooldown >= 1.f) {
		this->regCooldown = 0;
		if (this->hp < this->maxhp) {
			this->hp++;
			return true;
		}
	}
	return false;
}

void Player::attackLoad(const float& dt)
{
	if (this->attackCooldown < 1.f) {
		this->attackCooldown += this->attackSpeed * dt;
	}
}

void Player::ability(sf::RenderWindow& window, std::list<Projectile>& pociski, Projectile pocisk, const sf::Texture& texture, const sf::View& view, const float& dt)
{
	if (this->name == "Ninja") {
		pocisk.init(this->scale, "Shuriken", 2, 4, 3);
		pocisk.texture = texture;
		pocisk.sprite.setTexture(pocisk.texture);
		pocisk.sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
		pocisk.sprite.setScale(4 * this->scale, 4 * this->scale);

		pocisk.sprite.setPosition(this->sprite.getPosition().x + 32 * this->scale, this->sprite.getPosition().y + 32 * this->scale);
		pocisk.calculateVelocity(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)), dt);
		pocisk.sprite.setOrigin(2 * this->scale, 2 * this->scale);
		pociski.push_back(pocisk);
	}
	
}
