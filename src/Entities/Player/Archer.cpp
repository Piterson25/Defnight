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
    this->trappedTime = 10.f;
}

Archer::~Archer() = default;

void Archer::setAbilityTexture()
{
    if (this->name == "DARK") {
        this->ability.setTextureRect(sf::IntRect({48, 32}, {16, 16}));
    }
    else if (this->name == "PHANTOM") {
        this->ability.setTextureRect(sf::IntRect({64, 32}, {16, 16}));
    }
    else if (this->name == "PREDATOR") {
        this->ability.setTextureRect(sf::IntRect({80, 32}, {16, 16}));
    }
}

void Archer::endAbility()
{
    if (this->name == "DARK" || this->name == "PHANTOM" || this->name == "PREDATOR") {
        this->invisible = false;
    }
    if (this->name == "PREDATOR") {
        this->reg -= increasedReg;
        this->attack -= increasedAttack;
        this->attackSpeed -= increasedAttackSpeed;
        this->speed -= increasedSpeed;
        this->criticalChance -= increasedCriticalChance;
    }
}

void Archer::doAbility(SoundEngine &soundEngine)
{
    if (this->name == "HUNTER" || this->name == "RANGER" || this->name == "TRAPPER") {
        soundEngine.addSound("trap_place");
    }
    else if (this->name == "SHOOTER") {
        soundEngine.addSound("shuriken");
    }
    else if (this->name == "DARK" || this->name == "PHANTOM") {
        soundEngine.addSound("slowtime");
        this->invisible = true;
    }
    if (this->name == "PREDATOR") {
        soundEngine.addSound("predator");
        this->invisible = true;
        this->reg += increasedReg;
        this->attack += increasedAttack;
        this->attackSpeed += increasedAttackSpeed;
        this->speed += increasedSpeed;
        this->criticalChance += increasedCriticalChance;
    }
}

void Archer::upgradeAttributes(const std::string &t_name, sf::IntRect &intRect)
{
    this->name = t_name;
    if (this->name == "HUNTER") {
        intRect = sf::IntRect({48, 16}, {16, 16});
        this->setAttackSpeed(this->getAttackSpeed() + 1);
        this->abilityComponent->setAbilityStats(10.f, 10.f, 2.f);
    }
    else if (this->name == "RANGER") {
        intRect = sf::IntRect({64, 16}, {16, 16});
        this->setSpeed(this->getSpeed() + 1);
        this->abilityComponent->setAbilityStats(10.f, 10.f, 2.f);
    }
    else if (this->name == "TRAPPER") {
        intRect = sf::IntRect({80, 16}, {16, 16});
        this->setReg(this->getReg() + 1);
        this->abilityComponent->setAbilityStats(10.f, 10.f, 2.f);
    }
    else if (this->name == "SHOOTER") {
        intRect = sf::IntRect({48, 32}, {16, 16});
        this->setArmor(this->getArmor() + 1);
        this->abilityComponent->setAbilityStats(2.f, 2.f, 2.f);
    }
    else if (this->name == "DARK") {
        intRect = sf::IntRect({48, 48}, {16, 16});
        this->setReg(this->getReg() + 1);
        this->abilityComponent->setAbilityStats(20.f, 20.f, 5.f);
    }
    else if (this->name == "PHANTOM") {
        intRect = sf::IntRect({64, 48}, {16, 16});
        this->setAttack(this->getAttack() + 1);
        this->abilityComponent->setAbilityStats(20.f, 20.f, 5.f);
    }
    else if (this->name == "PREDATOR") {
        intRect = sf::IntRect({80, 48}, {16, 16});
        this->setCriticalChance(this->getCriticalChance() + 10);
        this->increasedReg = 3;
        this->increasedAttack = 5;
        this->increasedAttackSpeed = 2;
        this->increasedSpeed = 2;
        this->increasedCriticalChance = 30;
        this->abilityComponent->setAbilityStats(20.f, 20.f, 5.f);
    }
}
