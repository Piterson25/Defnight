#pragma once

#include "Tile.hpp"

class TileMap {
public:
    TileMap(sf::VideoMode &t_vm, const std::string &mapName);
    ~TileMap();

    const sf::FloatRect getBounds() const;
    const sf::Vector2f getMapSize() const;
    const size_t getSize() const;
    const sf::Vector2f getPosition(const size_t &index) const;
    const sf::FloatRect getGlobalBounds(const size_t &index) const;
    const std::vector<sf::FloatRect> getTilesGlobalBounds() const;

    void addTile(const std::string &name, const sf::Vector2f &size, float x,
                 float y);

    void drawMap(sf::RenderTarget &target);
    void drawObstacles(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    std::vector<std::unique_ptr<Tile>> tiles;
    std::vector<sf::FloatRect> tilesGlobalBounds;

    sf::Texture tilesTexture;
    sf::VertexArray vertexArray;
    sf::Sprite background;
    sf::Texture backgroundTexture;
    std::string mapName;
};
