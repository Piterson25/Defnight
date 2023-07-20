#pragma once

#include "Projectile.hpp"

class Shuriken : public Projectile {
public:
    Shuriken(const std::string &t_name, sf::VideoMode &t_vm,
             const sf::Vector2f &t_position, float difficulty_mod,
             const sf::Vector2f &coords, float coordsOffset, Player &player);
    ~Shuriken();

    void playerCollision(Player &player) override;
    void monsterCollision(Monster &monster, Player &player,
                          FloatingTextSystem &floatingTextSystem) override;
};
