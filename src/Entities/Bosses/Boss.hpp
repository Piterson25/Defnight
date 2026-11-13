#pragma once

#include "Entities/Monsters/Monster.hpp"

class Boss : public Monster {
public:
    Boss(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y, float difficulty_mod, float wave_mod,
         const std::vector<sf::FloatRect> &obstaclesBounds);
    virtual ~Boss();

    const uint32_t getReg() const;
    const bool getRegenerating() const;

    void setReg(uint32_t t_reg);
    void setRegenerating(bool t_regenerating);

    const bool isHPRegenerating(float dt);
    const bool isSpecialAttackReady() const;
    const bool isSpecialAttackAnimationDone() const;
    void resetSpecialAttack();
    void loadSpecialAttack(float dt);
    void specialAttackAnimation(float dt);
    void playSpawnSound(SoundEngine &soundEngine);

    virtual void specialAttack(SoundEngine &soundEngine, float dt) = 0;
    void update(float dt);
    void draw(sf::RenderTarget &target);
    void drawShadow(sf::RenderTarget &target);

protected:
    uint32_t reg;
    float regCooldown;
    bool regenerating;
    float specialAttackTimer;
    float specialAttackLimit;
    bool specialAttackAnimationReady;
};
