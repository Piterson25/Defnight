#include "Functions.h"
#include "Particle.h"

Particle::Particle(const sf::VideoMode& vm, const std::string& name, const float& x, const float& y, const std::uint32_t& attack)
	:vm(vm), name(name), attack(attack)
{
	if (this->name == "bomb")
		this->texture.loadFromFile("external/assets/explosion.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
	this->sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	this->sprite.setPosition(x - this->sprite.getGlobalBounds().width / 2.f, y - this->sprite.getGlobalBounds().height / 2.f);

	this->animationCooldown = 0;
	this->frame = 0;
	this->exploded = false;
}

Particle::~Particle()
{
}

const bool Particle::getExploded() const
{
	return this->exploded;
}

const uint32_t Particle::getAttack() const
{
	return this->attack;
}

const sf::FloatRect Particle::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

void Particle::update(const float& dt)
{
	if (!this->exploded) {
		this->animationCooldown += dt;
		if (this->animationCooldown >= 0.1f) {
			this->animationCooldown = 0.f;

			if (this->frame == 192) {
				this->exploded = true;
				this->frame = 192;
			}
			else {
				sf::IntRect intRect(this->frame, 0, 32, 32);
				this->sprite.setTextureRect(intRect);
				this->frame += 32;
			}
		}
	}
}

void Particle::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
