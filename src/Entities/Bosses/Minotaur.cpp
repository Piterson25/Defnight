#include "Minotaur.hpp"

Minotaur::Minotaur(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
                   float t_y, float difficulty_mod, float wave_mod,
                   const std::vector<sf::FloatRect> &obstaclesBounds)
    : Boss(t_name, t_vm, t_x, t_y, difficulty_mod, wave_mod, obstaclesBounds)
{
    this->name = "MINOTAUR";
    this->attack = static_cast<uint32_t>(7 * difficulty_mod);
    this->attackSpeed = 2;
    this->HP = static_cast<uint32_t>(100 * difficulty_mod);
    this->maxHP = static_cast<uint32_t>(100 * difficulty_mod);
    this->speed = 1;
    this->gold = static_cast<uint32_t>(50 * wave_mod);
    this->XP = static_cast<uint32_t>(200 * wave_mod);

    this->specialAttackLimit = 5.f;
    this->specialAttackTimer = 0.f;
}

Minotaur::~Minotaur() = default;

void Minotaur::specialAttack(SoundEngine &soundEngine, float dt)
{
    if (!this->soundPlayed) {
        soundEngine.addSound("punch");
        this->soundPlayed = true;
    }
}
