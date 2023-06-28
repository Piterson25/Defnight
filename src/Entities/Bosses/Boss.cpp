#include "Boss.hpp"

Boss::Boss(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
           float difficulty_mod, float wave_mod,
           const std::vector<sf::FloatRect> &obstaclesBounds)
    : Monster(t_name, t_vm, t_x, t_y, difficulty_mod, wave_mod,
              obstaclesBounds),
      specialAttackTimer(0.f), specialAttackLimit(5.f)
{
}

Boss::~Boss() = default;

const bool Boss::isSpecialAttackReady() const
{
    return this->specialAttackTimer >= this->specialAttackLimit;
}

void Boss::resetSpecialAttack()
{
    this->specialAttackTimer = 0.f;
}

void Boss::loadSpecialAttack(float dt)
{
    if (this->specialAttackTimer < this->specialAttackLimit) {
        this->specialAttackTimer += dt;
    }
}

void Boss::update(float dt)
{
    this->shadow.setPosition(this->getDownCenter().x -
                                 this->shadow.getTextureRect().width / 2 *
                                     this->shadow.getScale().x,
                             this->getDownCenter().y);
    if (this->soundPlayed && this->frame != 80) {
        this->soundPlayed = false;
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
