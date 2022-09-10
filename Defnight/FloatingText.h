#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

class FloatingText
{
public:
	FloatingText(sf::Font* font, const std::string& text, const unsigned& charSize,
		const float& posX, const float& posY, const sf::Color& color, sf::VideoMode& vm);
	virtual ~FloatingText();

	virtual const bool isOver() const;

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
private:
	sf::Font font;
	sf::Text text;
	sf::Vector2f velocity;

	float moveCooldown;
};

#endif