#pragma once

#include "Utils/Functions.hpp"

class Tile {
public:
    Tile(const std::string &name, const sf::Vector2f &size, float x, float y);
    ~Tile();

    const sf::Vector2f getPosition() const;
    const sf::FloatRect getGlobalBounds() const;

private:
    sf::FloatRect globalBounds;
    std::string name;
};
