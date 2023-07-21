#pragma once

#include "Utils/Functions.hpp"

class Particle {
public:
    Particle(sf::VideoMode &vm, const std::string &name, float x, float y,
             std::uint32_t attack, std::uint32_t area);
    ~Particle();

    const bool hasExploded() const;
    const uint32_t getAttack() const;
    const sf::FloatRect getGlobalBounds() const;

    void update(float dt);
    void draw(sf::RenderTarget &target);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::VideoMode &vm;
    std::string name;

    uint32_t attack;

    float animationCooldown;
    uint32_t frame;

    bool exploded;
};
