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
    this->projectileAttack = 3;
}

Warrior::~Warrior() = default;

void Warrior::setAbilityTexture()
{
    if (this->name == "knight") {
        this->ability.setTextureRect(sf::IntRect(0, 0, 16, 16));
    }
    else if (this->name == "scout") {
        this->ability.setTextureRect(sf::IntRect(16, 0, 16, 16));
    }
    else if (this->name == "crusader") {
        this->ability.setTextureRect(sf::IntRect(32, 0, 16, 16));
    }
    else if (this->name == "paladin") {
        this->ability.setTextureRect(sf::IntRect(48, 0, 16, 16));
    }
}

void Warrior::endAbility()
{
    if (this->name == "knight") {
        this->armor -= increasedArmor;
    }
    else if (this->name == "crusader") {
        this->attack -= 5;
        this->armor -= increasedArmor;
    }
    else if (this->name == "paladin") {
        this->reg -= 5;
        this->armor -= increasedArmor;
    }
}

void Warrior::doAbility(SoundEngine &soundEngine)
{
    if (this->name == "ninja") {
        soundEngine.addSound("shuriken");
    }
    else if (this->name == "knight") {
        this->increasedArmor = 5;
        this->armor += increasedArmor;
        soundEngine.addSound("ability");
    }
    else if (this->name == "scout") {
        soundEngine.addSound("ability");
    }
    else if (this->name == "sensei") {
        soundEngine.addSound("shuriken");
    }
    else if (this->name == "bomber") {
        soundEngine.addSound("shuriken");
    }
    else if (this->name == "crusader") {
        this->armor += increasedArmor;
        this->attack += 5;
        soundEngine.addSound("ability");
    }
    else if (this->name == "paladin") {
        this->armor += increasedArmor;
        this->reg += 5;
        soundEngine.addSound("ability");
    }
}

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
    else if (t_name == "SENSEI") {
        intRect = sf::IntRect(64, 0, 16, 16);
        this->setAttackSpeed(this->getAttackSpeed() + 1);
        this->setName("sensei");
        this->abilityComponent->setAbilityStats(2.f, 2.f, 2.f);
    }
    else if (t_name == "BOMBER") {
        intRect = sf::IntRect(80, 0, 16, 16);
        this->setCriticalChance(this->getCriticalChance() + 10);
        this->setProjectileArea(2);
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
