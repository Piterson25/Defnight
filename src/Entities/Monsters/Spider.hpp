#pragma once

#include "Monster.hpp"

class Spider : public Monster {
public:
    Spider(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
           float difficulty_mod, float wave_mod,
           const std::vector<sf::FloatRect> &obstaclesBounds);
    ~Spider();
};
