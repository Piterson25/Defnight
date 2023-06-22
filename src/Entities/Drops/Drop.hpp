#pragma once

#include "GUI/PlayerGUI.hpp"

class Drop : public Entity {
public:
    Drop(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
         uint32_t t_worth, bool t_vanishing);
    virtual ~Drop();

    const uint32_t getWorth() const;
    const bool hasSpawned() const;
    const bool hasVanished() const;

    void spawn(float dt);
    virtual void spin(float dt);
    void move(float posX, float posY, float dt);
    virtual const bool isPickedByPlayer(Player &player, PlayerGUI &playerGUI,
                                        FloatingTextSystem &floatingTextSystem,
                                        SoundEngine &soundEngine, float dt);

    void update(float dt);

protected:
    float spinCooldown;
    uint32_t worth;
    float angle;

    float vanishingCountdown;
    bool vanishing;
    bool spawned;
    float spawnCountdown;
};
