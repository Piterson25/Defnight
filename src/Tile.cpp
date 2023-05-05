#include "Functions.h"
#include "Tile.h"

Tile::Tile(const float& x, const float& y, const sf::Vector2f& size, const std::string& name)
	:globalBounds(sf::Vector2(x, y), sf::Vector2f(size)), name(name)
{

}

Tile::~Tile()
{
}

const sf::Vector2f Tile::getPosition() const
{
	return sf::Vector2f(this->globalBounds.left, this->globalBounds.top);
}

const sf::FloatRect Tile::getGlobalBounds() const
{
	return this->globalBounds;
}
