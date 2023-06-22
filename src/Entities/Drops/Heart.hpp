#pragma once

#include "Drop.hpp"

class Heart : public Drop {
public:
    Heart(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
          uint32_t t_worth, bool t_vanishing);
    ~Heart();

    void spin(float dt);
    const bool isPickedByPlayer(Player &player, PlayerGUI &playerGUI,
                                FloatingTextSystem &floatingTextSystem,
                                SoundEngine &soundEngine, float dt);
};
