#include "Orc.hpp"

Orc::Orc(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
         float difficulty_mod, float wave_mod,
         const std::vector<sf::FloatRect> &obstaclesBounds)
    : Monster(t_name, t_vm, t_x, t_y, difficulty_mod, wave_mod, obstaclesBounds)
{
    this->name = "orc";
    this->attack = static_cast<uint32_t>(3 * difficulty_mod);
    this->attackSpeed = 2;
    this->HP = static_cast<uint32_t>(12 * difficulty_mod);
    this->maxHP = static_cast<uint32_t>(12 * difficulty_mod);
    this->speed = 2;
    this->gold = static_cast<uint32_t>(3 * wave_mod);
    this->XP = static_cast<uint32_t>(17 * wave_mod);
}

Orc::~Orc() = default;
