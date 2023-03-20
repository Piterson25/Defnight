#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include "stdafx.h"

class FloatingText
{
public:
	FloatingText(sf::Font* font, const std::string& text, const unsigned& charSize,
		const float& posX, const float& posY, const sf::Color& color, const bool& isgui, sf::VideoMode& vm);
	virtual ~FloatingText();

	virtual const bool isOver() const;
	virtual const bool isGui() const;

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
private:
	sf::Font* font;
	sf::Text text;
	sf::Vector2f velocity;
	bool gui;

	float moveCooldown;
};

#endif