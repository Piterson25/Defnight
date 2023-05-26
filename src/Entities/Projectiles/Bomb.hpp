#pragma once

#include "Projectile.hpp"

class Bomb : public Projectile {
public:
    Bomb(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
         float difficulty_mod, const sf::Vector2f &coords, float coordsOffset);
    ~Bomb();

    void playerCollision(Player &player) override;
    void monsterCollision(Monster &monster, Player &player,
                          FloatingTextSystem &floatingTextSystem) override;
};
