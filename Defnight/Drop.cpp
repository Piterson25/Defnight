#include "Functions.h"
#include "Drop.h"

Drop::Drop()
{
	this->scale = 1.f;
	this->spinCooldown = 0.f;
	this->worth = 0;
	this->predkosc = sf::Vector2f(0.f, 0.f);
	this->angle = 0.f;
}

Drop::~Drop()
{

}

void Drop::init(const float& scale, const sf::Vector2f& position, const std::string& name, const unsigned& worth)
{
	this->scale = scale;
	createSprite(this->sprite, this->texture, 2 * this->scale);
	this->sprite.setPosition(position);
	this->worth = worth;
	this->name = name;
}

void Drop::spin(const float& dt)
{
	this->spinCooldown += dt;
	if (this->spinCooldown > 0.166f) {
		const int w = this->sprite.getTextureRect().left;
		if (this->name == "coin") {
			if (w == 80) this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
			else this->sprite.setTextureRect(sf::IntRect(w + 16, 0, 16, 16));
		}
		else {
			if (w == 80) this->sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
			else this->sprite.setTextureRect(sf::IntRect(w + 16, 16, 16, 16));
		}
		
		this->spinCooldown = 0.f;
	}
}

void Drop::move(const float& dt)
{
	this->predkosc.x = 256.f * dt * this->scale * cos((3.1415f / 180) * this->angle);
	this->predkosc.y = 256.f * dt * this->scale * sin((3.1415f / 180) * this->angle);
	this->sprite.move(this->predkosc);
}
