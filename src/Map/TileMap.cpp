#include "TileMap.hpp"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
    this->tiles.clear();
}

const size_t TileMap::getSize() const
{
    return this->tiles.size();
}

const sf::Vector2f TileMap::getPosition(const size_t &index) const
{
    return this->tiles[index]->getPosition();
}

const sf::FloatRect TileMap::getGlobalBounds(const size_t &index) const
{
    return this->tiles[index]->getGlobalBounds();
}

const std::vector<sf::FloatRect> TileMap::getTilesGlobalBounds() const
{
    return this->tilesGlobalBounds;
}

void TileMap::addTile(const std::string &name, const sf::Vector2f &size,
                      float x, float y)
{
    this->tiles.emplace_back(new Tile(name, size, x, y));
    this->tilesGlobalBounds.emplace_back(
        sf::FloatRect(sf::Vector2f(x, y), size));
}
