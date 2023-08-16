#include "Tile.hpp"

Tile::Tile(const std::string &name, const sf::Vector2f &size, float x, float y)
    : globalBounds(sf::Vector2(x, y), sf::Vector2f(size)), name(name)
{
}

Tile::~Tile() = default;

const sf::Vector2f Tile::getPosition() const
{
    return sf::Vector2f(this->globalBounds.left, this->globalBounds.top);
}

const sf::FloatRect Tile::getGlobalBounds() const
{
    return this->globalBounds;
}
