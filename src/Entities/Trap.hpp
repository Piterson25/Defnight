#pragma once

#include "Systems/MonsterSystem.hpp"

enum class TrapState { OPENED, CLOSING, CLOSED, BROKEN };
enum class TrapType { NORMAL, DAMAGE_DEALING, SCENTED };

struct TrapStats {
    TrapType type;
    float releaseTimeLimit;
    uint32_t attack;
    float attackTimeLimit;
};

class Trap : public Entity {
public:
    Trap(sf::VideoMode &t_vm, const sf::Vector2f &t_position, TrapStats t_stats);
    ~Trap();

    const TrapType getType() const;
    const uint32_t getAttack() const;
    const bool isBroken() const;
    const bool isDamageDealing() const;
    void checkSteppedOn(Monster *monster, SoundEngine &soundEngine, float dt);
    void dealDamage(PlayerGUI &playerGUI, FloatingTextSystem &floatingTextSystem, float dt);

    void update(float dt);
    void drawShadow(sf::RenderTarget &target);

protected:
    sf::Sprite shadow;
    sf::Texture shadow_texture;

    TrapState state;
    TrapType type;

    float closingCountdown;
    float releaseCooldown;
    float releaseTimeLimit;

    int attack;
    float attackCooldown;
    float attackTimeLimit;

    Monster *trappedMonster;
};
