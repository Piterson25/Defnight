#pragma once

#include "Entities/Player/Player.hpp"
#include "Systems/FloatingTextSystem.hpp"

class AbilityUpgradeGUI {
public:
    AbilityUpgradeGUI(sf::VideoMode &t_vm, Player &t_player);
    ~AbilityUpgradeGUI();

    void setAbility(const sf::IntRect &intRect);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;

    gui::Sprite *abilitySprite;
};
