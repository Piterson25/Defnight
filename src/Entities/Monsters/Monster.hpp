#pragma once

#include "Components/AIComponent.hpp"
#include "Entities/Player/Player.hpp"
#include "Systems/FloatingTextSystem.hpp"

class FloatingTextSystem;

class Monster : public Entity {
public:
    Monster(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
            float t_y, float difficulty_mod, float wave_mod,
            const std::vector<sf::FloatRect> &obstaclesBounds);
    virtual ~Monster();

    const float getDifficultyMod() const;
    const uint32_t getGold() const;
    const bool hasSpawned() const;
    const bool hasDeadCountdownExpired() const;

    void setGold(uint32_t gold);

    virtual const bool
    hasAttackedPlayer(const std::vector<sf::FloatRect> &obstaclesBounds,
                      Player &player, SoundEngine &soundEngine,
                      FloatingTextSystem &floatingTextSystem);
    const bool hasLineOfSight(const std::vector<sf::FloatRect> &obstaclesBounds,
                              const sf::Vector2f &a_p1,
                              const sf::Vector2f &a_p2);

    void dyingAnimation(float dt);

    void calculateAI(const std::vector<sf::FloatRect> &obstaclesBounds,
                     Player &player, const std::vector<sf::Vector2f> &positions,
                     float dt);

    void spawn(float dt);
    virtual void update(float dt);
    void draw(sf::RenderTarget &target);
    virtual void drawShadow(sf::RenderTarget &target);

protected:
    virtual const bool
    canAttackPlayer(const std::vector<sf::FloatRect> &obstaclesBounds,
                    Player &player);

    sf::Sprite shadow;
    sf::Texture shadow_texture;

    float difficultyMod;
    uint32_t gold;

    bool spawned;
    float spawnCountdown;
    float deadCountdown;

    bool soundPlayed;

    std::unique_ptr<AIComponent> AI;
};
