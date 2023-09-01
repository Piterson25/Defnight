#include "Particle.hpp"

Particle::Particle(sf::VideoMode &vm, const std::string &name, float x, float y,
                   std::uint32_t attack, std::uint32_t area)
    : vm(vm), name(name), attack(attack)
{
    if (this->name == "BOMB") {
        this->texture.loadFromFile("assets/textures/explosion.png");
    }
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(calcScale(4 * static_cast<float>(area) / 2.f, vm),
                          calcScale(4 * static_cast<float>(area) / 2.f, vm));
    this->sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    this->sprite.setPosition(x - this->sprite.getGlobalBounds().width / 2.f,
                             y - this->sprite.getGlobalBounds().height / 2.f);

    this->animationCooldown = 0;
    this->frame = 0;
    this->exploded = false;
}

Particle::~Particle()
{
}

const bool Particle::hasExploded() const
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

void Particle::update(float dt)
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

void Particle::draw(sf::RenderTarget &target)
{
    target.draw(this->sprite);
}
