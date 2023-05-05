#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "Tile.hpp"

class TileMap
{
public:
	TileMap();
	virtual ~TileMap();

	virtual const size_t getSize() const;
	virtual const sf::Vector2f getPosition(const size_t& index) const;
	virtual const sf::FloatRect getGlobalBounds(const size_t& index) const;

	void addTile(const float& x, const float& y, const sf::Vector2f& size, const std::string& name);

private:
	std::vector<std::unique_ptr<Tile>> tiles;
};

#endif