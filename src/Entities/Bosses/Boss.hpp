#pragma once

#include "Entities/Monsters/Monster.hpp"

class Boss : public Monster {
public:
    Boss(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
         float difficulty_mod, float wave_mod,
         const std::vector<sf::FloatRect> &obstaclesBounds);
    virtual ~Boss();

    const bool isSpecialAttackReady() const;
    const bool isSpecialAttackAnimationDone() const;
    void resetSpecialAttack();
    void loadSpecialAttack(float dt);
    void specialAttackAnimation(float dt);

    virtual void specialAttack(SoundEngine &soundEngine, float dt) = 0;
    void update(float dt);
    void draw(sf::RenderTarget &target);
    void drawShadow(sf::RenderTarget &target);

protected:
    float specialAttackTimer;
    float specialAttackLimit;
    bool specialAttackAnimationReady;
};
