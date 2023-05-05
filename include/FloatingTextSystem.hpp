#ifndef FLOATINGTEXTSYSTEM_HPP
#define FLOATINGTEXTSYSTEM_HPP

#include "FloatingText.hpp"

class FloatingText;

class FloatingTextSystem
{
public:
	FloatingTextSystem(sf::Font* font, const sf::VideoMode& vm);
	virtual ~FloatingTextSystem();
	void addFloatingText(const std::string& text, const unsigned& charSize,
		const float& posX, const float& posY, const sf::Color& color, const bool& isgui);

	void draw(sf::RenderTarget& target);
	void drawGui(sf::RenderTarget& target);
	void update(const float& dt);
private:
	std::list<std::unique_ptr<FloatingText>> floatingTexts;
	sf::Font* font;
	sf::VideoMode vm;
};

#endif