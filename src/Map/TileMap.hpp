#pragma once

#include "Tile.hpp"

class TileMap {
public:
    TileMap();
    ~TileMap();

    const size_t getSize() const;
    const sf::Vector2f getPosition(const size_t &index) const;
    const sf::FloatRect getGlobalBounds(const size_t &index) const;
    const std::vector<sf::FloatRect> getTilesGlobalBounds() const;

    void addTile(const std::string &name, const sf::Vector2f &size, float x, float y);

private:
    std::vector<std::unique_ptr<Tile>> tiles;
    std::vector<sf::FloatRect> tilesGlobalBounds;
};
