#include "Functions.h"
#include "Text.h"

Text::Text()
{

}

Text::~Text()
{

}

void Text::init(float& scale, std::string nazwa, const sf::Vector2f& position, const sf::Font& font, const sf::Uint8& R, const sf::Uint8& G, const sf::Uint8& B)
{
	this->scale = scale;
	createText(this->text, font, 16 * this->scale, nazwa, position.x, position.y, R, G, B);
}

void Text::move(const float& dt)
{
	this->text.move(0, -32.f * dt * this->scale);
}
