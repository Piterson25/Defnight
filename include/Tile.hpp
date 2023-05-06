#ifndef TILE_HPP
#define TILE_HPP

#include "header.h"

class Tile {
public:
    Tile(const float &x, const float &y, const sf::Vector2f &size, const std::string &name);
    virtual ~Tile();

    virtual const sf::Vector2f getPosition() const;
    virtual const sf::FloatRect getGlobalBounds() const;

private:
    sf::FloatRect globalBounds;
    std::string name;
};

#endif