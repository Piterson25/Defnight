#pragma once

#include "Player.hpp"

class Warrior : public Player {
public:
    Warrior(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
            float t_y);
    ~Warrior();

    void setAbilityTexture();
    void endAbility();
    void doAbility(SoundEngine &soundEngine);

    void upgradeAttributes(const std::string &t_name, sf::IntRect &intRect);
};
