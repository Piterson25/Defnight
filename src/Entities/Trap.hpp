#pragma once

#include "Systems/MonsterSystem.hpp"

enum class TrapState { OPENED, CLOSING, CLOSED };

class Trap : public Entity {
public:
    Trap(sf::VideoMode &t_vm, const sf::Vector2f &t_position, uint32_t t_attack);
    ~Trap();

    const TrapState getState() const;
    const uint32_t getAttack() const;
    void checkSteppedOn(Monster &monster, PlayerGUI &playerGUI, FloatingTextSystem &floatingTextSystem,
                        SoundEngine &soundEngine, float dt);

    void update(float dt);
    void drawShadow(sf::RenderTarget &target);

protected:
    sf::Sprite shadow;
    sf::Texture shadow_texture;

    int attack;
    TrapState state;

    float closingCountdown;
};
