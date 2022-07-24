#pragma once
#include "Drop.h"

class Text
{
public:
	sf::Text text;

	float scale;
	float move_cd;

	void init(float& scale, std::string nazwa, const sf::Vector2f& position, const sf::Font& font, const sf::Uint8& R, const sf::Uint8& G, const sf::Uint8& B);

	Text();
	virtual ~Text();

	void move(const float& dt);
};

