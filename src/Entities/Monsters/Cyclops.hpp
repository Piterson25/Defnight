#pragma once

#include "Monster.hpp"

class Cyclops : public Monster {
public:
    Cyclops(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
            float t_y, float difficulty_mod, float wave_mod,
            const std::vector<sf::FloatRect> &obstaclesBounds);
    ~Cyclops();

    const bool
    hasAttackedPlayer(const std::vector<sf::FloatRect> &obstaclesBounds,
                      Player &player, SoundEngine &soundEngine,
                      FloatingTextSystem &floatingTextSystem) override;

private:
    const bool
    canAttackPlayer(const std::vector<sf::FloatRect> &obstaclesBounds,
                    Player &player) override;
};
