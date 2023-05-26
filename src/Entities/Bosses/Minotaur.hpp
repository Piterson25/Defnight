#pragma once

#include "Boss.hpp"

class Minotaur : public Boss {
public:
    Minotaur(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
             float t_y, float difficulty_mod, float wave_mod,
             const std::vector<sf::FloatRect> &obstaclesBounds);
    ~Minotaur();

    void specialAttack(SoundEngine &soundEngine, float dt);
};
