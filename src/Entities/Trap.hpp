#pragma once

#include "Systems/MonsterSystem.hpp"

enum class TrapState { OPENED, CLOSING, CLOSED, BROKEN };
enum class TrapType { NORMAL, DAMAGE_DEALING, SCENTED };

struct TrapStats {
    TrapType type;
    uint32_t attack;
    float releaseLimit;
    float damageLimit;
};

class Trap : public Entity {
public:
    Trap(sf::VideoMode &t_vm, const sf::Vector2f &t_position, TrapStats t_stats);
    ~Trap();

    const TrapState getState() const;
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
    int attack;

    float closingCountdown;
    float releaseCooldown;
    float releaseLimit;
    float damageCooldown;
    float damageLimit;

    Monster *trappedMonster;
};
