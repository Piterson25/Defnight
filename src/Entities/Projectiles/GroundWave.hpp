#pragma once

#include "Projectile.hpp"

class GroundWave : public Projectile {
public:
    GroundWave(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
               float t_y, float difficulty_mod, const sf::Vector2f &coords,
               float coordsOffset);
    ~GroundWave();

    void playerCollision(Player &player) override;
    void monsterCollision(Monster &monster, Player &player,
                          FloatingTextSystem &floatingTextSystem) override;

    void update(float dt) override;
    void draw(sf::RenderTarget &target);
};
