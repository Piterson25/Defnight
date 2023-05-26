#include "Boss.hpp"

Boss::Boss(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
           float difficulty_mod, float wave_mod,
           const std::vector<sf::FloatRect> &obstaclesBounds)
    : Monster(t_name, t_vm, t_x, t_y, difficulty_mod, wave_mod,
              obstaclesBounds),
      specialAttackCountdown(0.f), specialAttackCooldown(5.f)
{
}

Boss::~Boss() = default;

const bool Boss::specialReady() const
{
    return this->specialAttackCountdown >= this->specialAttackCooldown;
}

void Boss::resetSpecialAttack()
{
    this->specialAttackCountdown = 0.f;
}

void Boss::loadSpecialAttack(float dt)
{
    if (this->specialAttackCountdown < this->specialAttackCooldown) {
        this->specialAttackCountdown += dt;
    }
}

void Boss::specialAttack(float dt)
{
    this->specialAttackCountdown += dt;
    if (this->specialAttackCountdown >= 5.f) {
        this->specialAttackCountdown = 0.f;
    }
}

void Boss::update(float dt)
{
    this->shadow.setPosition(
        this->sprite.getPosition().x,
        this->sprite.getPosition().y +
            calcY(static_cast<float>(52 * this->entitySize), this->vm));
    if (this->playedSound && this->frame != 80) {
        this->playedSound = false;
    }

    this->loadSpecialAttack(dt);
}

void Boss::draw(sf::RenderTarget &target)
{
    target.draw(this->sprite);
}

void Boss::drawShadow(sf::RenderTarget &target)
{
    target.draw(this->shadow);
}
