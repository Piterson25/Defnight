#pragma once

#include "Audio/SoundEngine.hpp"
#include "Entities/Player/Player.hpp"
#include "GUI/PlayerGUI.hpp"
#include "Systems/FloatingTextSystem.hpp"
#include "Utils/Functions.hpp"

class Player;
class PlayerGUI;
class FloatingTextSystem;
class SoundEngine;

class Drop {
public:
    Drop(sf::VideoMode &vm, const std::string &name, float posX, float posY,
         uint32_t worth, bool vanishing);
    ~Drop();

    const std::string getName() const;
    const uint32_t getWorth() const;
    const sf::Vector2f getPosition() const;
    const bool hasSpawned() const;
    const bool hasVanished() const;

    void spawn(float dt);
    void spin(float dt);
    void move(float posX, float posY, float dt);
    const bool isPickedByPlayer(Player &player, PlayerGUI &playerGUI,
                                FloatingTextSystem &floatingTextSystem,
                                SoundEngine &soundEngine, float dt);

    void update(float dt);
    void draw(sf::RenderTarget &target);

private:
    sf::Texture texture;
    sf::Sprite sprite;

    sf::Vector2f velocity;
    std::string name;

    float spinCooldown;
    uint32_t worth;
    float angle;

    float vanishingCountdown;
    bool vanishing;
    bool spawned;
    float spawnCountdown;

    sf::VideoMode &vm;
};
