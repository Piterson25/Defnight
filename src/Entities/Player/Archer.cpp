#include "Archer.hpp"

Archer::Archer(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y) : Player(t_name, t_vm, t_x, t_y)
{
    this->attackType = AttackType::RANGED;
    this->name = "ARCHER";
    this->attack = 3;
    this->attackSpeed = 3;
    this->maxHP = 10;
    this->HP = 10;
    this->reg = 2;
    this->speed = 5;
    this->armor = 2;
    this->sprint = 200;
    this->maxSprint = 200;
    this->criticalChance = 10;
    this->projectileAttack = 3;
}

Archer::~Archer() = default;

void Archer::setAbilityTexture()
{
}

void Archer::endAbility()
{
}

void Archer::doAbility(SoundEngine &soundEngine)
{
}

void Archer::upgradeAttributes(const std::string &t_name, sf::IntRect &intRect)
{
}
