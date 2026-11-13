#pragma once

#include "Player.hpp"

class Archer : public Player {
public:
    Archer(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y);
    ~Archer();

    void setAbilityTexture() override;
    void endAbility() override;
    void doAbility(SoundEngine &soundEngine) override;

    void upgradeAttributes(const std::string &t_name, sf::IntRect &intRect) override;
};
