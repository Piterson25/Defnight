#include "Spider.hpp"

Spider::Spider(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
               float t_y, float difficulty_mod, float wave_mod,
               const std::vector<sf::FloatRect> &obstaclesBounds)
    : Monster(t_name, t_vm, t_x, t_y, difficulty_mod, wave_mod, obstaclesBounds)
{
    this->name = "SPIDER";
    this->attack = static_cast<uint32_t>(2 * difficulty_mod);
    this->attackSpeed = 3;
    this->HP = static_cast<uint32_t>(7 * difficulty_mod);
    this->maxHP = static_cast<uint32_t>(7 * difficulty_mod);
    this->speed = 3;
    this->gold = static_cast<uint32_t>(2 * wave_mod);
    this->XP = static_cast<uint32_t>(10 * wave_mod);
}

Spider::~Spider() = default;
