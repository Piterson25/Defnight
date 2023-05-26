#include "Warrior.hpp"

Warrior::Warrior(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
                 float t_y)
    : Player(t_name, t_vm, t_x, t_y)
{
    this->name = "warrior";
    this->attack = 5;
    this->attackSpeed = 4;
    this->maxHP = 10;
    this->HP = 10;
    this->reg = 1;
    this->speed = 4;
    this->armor = 3;
    this->criticalChance = 20;
}

Warrior::~Warrior() = default;
