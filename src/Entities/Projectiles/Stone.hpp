#pragma once

#include "Projectile.hpp"

class Stone : public Projectile {
public:
    Stone(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
          float difficulty_mod, const sf::Vector2f &coords, float coordsOffset);
    ~Stone();

    void playerCollision(Player &player) override;
    void monsterCollision(Monster &monster, Player &player,
                          FloatingTextSystem &floatingTextSystem) override;
};
