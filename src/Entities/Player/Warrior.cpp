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

void Warrior::upgradeAttributes(const std::string &t_name, sf::IntRect &intRect)
{
    if (t_name == "NINJA") {
        intRect = sf::IntRect(16, 0, 16, 16);
        this->setSpeed(this->getSpeed() + 1);
        this->setName("ninja");
        this->abilityComponent->setAbilityStats(2.f, 2.f, 2.f);
    }
    else if (t_name == "KNIGHT") {
        intRect = sf::IntRect(32, 0, 16, 16);
        this->setAttack(this->getAttack() + 1);
        this->setName("knight");
        this->abilityComponent->setAbilityStats(20.f, 20.f, 5.f);
    }
    else if (t_name == "SCOUT") {
        intRect = sf::IntRect(48, 0, 16, 16);
        this->setReg(this->getReg() + 1);
        this->setName("scout");
        this->abilityComponent->setAbilityStats(10.f, 10.f, 5.f);
    }
    else if (t_name == "MASTER") {
        intRect = sf::IntRect(64, 0, 16, 16);
        this->setAttackSpeed(this->getAttackSpeed() + 1);
        this->setName("master");
        this->abilityComponent->setAbilityStats(2.f, 2.f, 2.f);
    }
    else if (t_name == "BOMBER") {
        intRect = sf::IntRect(80, 0, 16, 16);
        this->setCriticalChance(this->getCriticalChance() + 10);
        this->setName("bomber");
        this->abilityComponent->setAbilityStats(2.f, 2.f, 2.f);
    }
    else if (t_name == "CRUSADER") {
        intRect = sf::IntRect(96, 0, 16, 16);
        this->setMaxHP(this->getMaxHP() + 2);
        this->setName("crusader");
        this->abilityComponent->setAbilityStats(20.f, 20.f, 10.f);
    }
    else if (t_name == "PALADIN") {
        intRect = sf::IntRect(112, 0, 16, 16);
        this->setAttackSpeed(this->getAttackSpeed() + 1);
        this->setName("paladin");
        this->abilityComponent->setAbilityStats(20.f, 20.f, 10.f);
    }
}
