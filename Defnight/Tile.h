#ifndef TILE_H
#define TILE_H

class Tile
{
public:
	Tile(const float& x, const float& y, const sf::Vector2f& size, const std::string& name);
	virtual ~Tile();

	virtual const sf::Vector2f getPosition() const;
	virtual const sf::FloatRect getGlobalBounds() const;

private:
	std::string name;
	sf::FloatRect globalBounds;
};

#endif