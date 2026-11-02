#include "Archer.hpp"

Archer::Archer(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y) : Player(t_name, t_vm, t_x, t_y)
{
    this->attackType = AttackType::RANGED;
    this->heroName = HeroName::ARCHER;
    this->name = "ARCHER";
    this->attack = 3;
    this->attackSpeed = 2;
    this->maxHP = 10;
    this->HP = 10;
    this->reg = 2;
    this->speed = 5;
    this->armor = 1;
    this->sprint = 150;
    this->maxSprint = 150;
    this->criticalChance = 10;
    this->projectileAttack = 3;
    this->trapAttack = 5;
}

Archer::~Archer() = default;

void Archer::setAbilityTexture()
{
    if (this->name == "DARK") {
        this->ability.setTextureRect(sf::IntRect({48, 0}, {16, 16}));
    }
}

void Archer::endAbility()
{
    if (this->name == "DARK") {
        this->invisible = false;
    }
}

void Archer::doAbility(SoundEngine &soundEngine)
{
    if (this->name == "DARK") {
        soundEngine.addSound("slowtime");
        this->invisible = true;
    }
    else if (this->name == "SHOOTER") {
        soundEngine.addSound("shuriken");
    }
    else if (this->name == "RANGER") {
        soundEngine.addSound("trap_place");
    }
}

void Archer::upgradeAttributes(const std::string &t_name, sf::IntRect &intRect)
{
    this->name = t_name;
    if (t_name == "DARK") {
        intRect = sf::IntRect({48, 16}, {16, 16});
        this->setReg(this->getReg() + 1);
        this->abilityComponent->setAbilityStats(20.f, 20.f, 5.f);
    }
    else if (t_name == "SHOOTER") {
        intRect = sf::IntRect({48, 32}, {16, 16});
        this->setArmor(this->getArmor() + 1);
        this->abilityComponent->setAbilityStats(2.f, 2.f, 2.f);
    }
    else if (t_name == "RANGER") {
        intRect = sf::IntRect({48, 48}, {16, 16});
        this->setSpeed(this->getSpeed() + 1);
        this->abilityComponent->setAbilityStats(10.f, 10.f, 2.f);
    }
}
