#pragma once

#include "Player.hpp"

class Warrior : public Player {
public:
    Warrior(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
            float t_y);
    ~Warrior();
};
