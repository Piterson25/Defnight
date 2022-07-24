#pragma once
#include "Functions.h"

class Entity
{
protected:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Vector2f velocity;
public:
	Entity();
	virtual ~Entity();
};
