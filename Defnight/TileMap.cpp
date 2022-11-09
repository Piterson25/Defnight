#include "stdafx.h"
#include "TileMap.h"

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

const sf::Vector2f TileMap::getPosition(const size_t& index) const
{
	return this->tiles[index]->getPosition();
}

const sf::FloatRect TileMap::getGlobalBounds(const size_t& index) const
{
	return this->tiles[index]->getGlobalBounds();
}

void TileMap::addTile(const float& x, const float& y, const sf::Vector2f& size, const std::string& name)
{
	this->tiles.emplace_back(new Tile(x, y, size, name));
}

