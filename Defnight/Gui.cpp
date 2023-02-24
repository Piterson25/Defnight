#include "stdafx.h"
#include "Functions.h"
#include "Gui.h"

gui::ButtonText::ButtonText(sf::Font* font, const std::string& text, const unsigned& charSize, 
	const float& posX, const float& posY, const sf::Color& idleColor, const sf::Color& hoverColor, const bool& center)
{
	this->buttonState = BUTTON_IDLE;
	this->font = font;
	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setCharacterSize(charSize);
	this->text.setFillColor(idleColor);
	this->text.setPosition(posX, posY);
	if (center) this->center(posX);
}

gui::ButtonText::~ButtonText()
{

}

const bool gui::ButtonText::isPressed() const
{
	if (this->buttonState == BUTTON_PRESSED) return true;
	return false;
}

const std::string gui::ButtonText::getText() const
{
	return this->text.getString();
}

const float gui::ButtonText::getWidth() const
{
	return this->text.getGlobalBounds().width;
}

void gui::ButtonText::setPosition(const sf::Vector2f& position)
{
	this->text.setPosition(position);
}

void gui::ButtonText::setText(const std::string& text)
{
	this->text.setString(text);
}

void gui::ButtonText::center(const float& posX)
{
	this->text.setPosition(static_cast<float>(static_cast<int>(posX - (this->text.getGlobalBounds().width) / 2)), this->text.getPosition().y);
}

void gui::ButtonText::update(const sf::Vector2i& mousePosWindow)
{
	this->buttonState = BUTTON_IDLE;

	if (this->text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
		this->buttonState = BUTTON_HOVER;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->buttonState = BUTTON_PRESSED;
		}
	}

	switch (this->buttonState)
	{
	case BUTTON_IDLE:
		this->text.setFillColor(this->idleColor);
		break;
	case BUTTON_HOVER:
		this->text.setFillColor(this->hoverColor);
		break;
	case BUTTON_PRESSED:
		this->text.setFillColor(this->hoverColor);
		break;
	}
}

void gui::ButtonText::draw(sf::RenderTarget& target)
{
	target.draw(this->text);
}

gui::Text::Text(sf::Font* font, const std::string& text, const unsigned& charSize,
	const float& posX, const float& posY, const sf::Color& color, const bool& center)
{
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setCharacterSize(charSize);
	this->text.setFillColor(color);
	this->text.setPosition(posX, posY);
	if (center) this->center(posX);
}

gui::Text::~Text()
{

}

const std::string gui::Text::getText() const
{
	return this->text.getString();
}

const float gui::Text::getWidth() const
{
	return this->text.getGlobalBounds().width;
}

void gui::Text::setPosition(const sf::Vector2f& position)
{
	this->text.setPosition(position);
}

void gui::Text::setFillColor(const sf::Color& color)
{
	this->text.setFillColor(color);
}

void gui::Text::setText(const std::string& text)
{
	this->text.setString(text);
}

void gui::Text::center(const float& posX)
{
	this->text.setPosition(static_cast<float>(static_cast<int>(posX - (this->text.getGlobalBounds().width) / 2)), this->text.getPosition().y);
}

void gui::Text::draw(sf::RenderTarget& target)
{
	target.draw(this->text);
}



gui::ButtonSprite::ButtonSprite(const std::string& texturePath, const float& posX, const float& posY, const float& scale, const bool& center)
{
	this->buttonState = BUTTON_IDLE;
	this->texture.loadFromFile(texturePath);
	this->sprite.setTexture(this->texture);
	this->sprite.setColor(sf::Color::Transparent);
	this->sprite.setPosition(posX, posY);
	this->sprite.setScale(scale, scale);
	if (center) this->center(posX);
}

gui::ButtonSprite::ButtonSprite(const sf::Texture& texture, const float& posX, const float& posY, const float& scale, const bool& center)
{
	this->buttonState = BUTTON_IDLE;
	this->texture = texture;
	this->sprite.setTexture(this->texture);
	this->sprite.setColor(sf::Color::Transparent);
	this->sprite.setPosition(posX, posY);
	this->sprite.setScale(scale, scale);
	if (center) this->center(posX);
}

gui::ButtonSprite::~ButtonSprite()
{

}

const sf::IntRect gui::ButtonSprite::getTextureRect() const
{
	return this->sprite.getTextureRect();
}

const bool gui::ButtonSprite::isPressed() const
{
	if (this->buttonState == BUTTON_PRESSED) return true;
	return false;
}

void gui::ButtonSprite::setTransparent()
{
	this->buttonState = BUTTON_IDLE;
	this->sprite.setColor(sf::Color::Transparent);
}

void gui::ButtonSprite::setTextureRect(const sf::IntRect& intRect)
{
	this->sprite.setTextureRect(intRect);
}

void gui::ButtonSprite::center(const float& posX)
{
	this->sprite.setPosition(static_cast<float>(static_cast<int>(posX - (this->sprite.getGlobalBounds().width) / 2)), this->sprite.getPosition().y);
}

void gui::ButtonSprite::update(const sf::Vector2i& mousePosWindow)
{
	this->buttonState = BUTTON_IDLE;

	if (this->sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
		this->buttonState = BUTTON_HOVER;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->buttonState = BUTTON_PRESSED;
		}
	}

	switch (this->buttonState)
	{
	case BUTTON_IDLE:
		this->sprite.setColor(sf::Color::Transparent);
		break;
	case BUTTON_HOVER:
		this->sprite.setColor(sf::Color(255, 255, 255));
		break;
	case BUTTON_PRESSED:
		this->sprite.setColor(sf::Color(255, 255, 255));
		break;
	}
}

void gui::ButtonSprite::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}

gui::Sprite::Sprite(const std::string& texturePath, const float& posX, const float& posY, const float& scale, const bool& center)
{
	this->texture.loadFromFile(texturePath);
	this->sprite.setTexture(this->texture);
	this->sprite.setPosition(posX, posY);
	this->sprite.setScale(scale, scale);
	if (center) this->center(posX);
}

gui::Sprite::Sprite(const sf::Texture& texture, const float& posX, const float& posY, const float& scale, const bool& center)
{
	this->texture = texture;
	this->sprite.setTexture(this->texture);
	this->sprite.setPosition(posX, posY);
	this->sprite.setScale(scale, scale);
	if (center) this->center(posX);
}

gui::Sprite::Sprite(sf::Sprite& sprite, const float& posX, const float& posY, const float& scale, const bool& center)
	:sprite(sprite)
{
	this->texture = *sprite.getTexture();
	this->sprite.setPosition(posX, posY);
	this->sprite.setScale(scale, scale);
	if (center) this->center(posX);
}

gui::Sprite::~Sprite()
{

}

const sf::FloatRect gui::Sprite::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

const sf::IntRect gui::Sprite::getTextureRect() const
{
	return this->sprite.getTextureRect();
}

void gui::Sprite::setColor(const sf::Color& color)
{
	this->sprite.setColor(color);
}

void gui::Sprite::setTextureRect(const sf::IntRect& intRect)
{
	this->sprite.setTextureRect(intRect);
}

void gui::Sprite::center(const float& posX)
{
	this->sprite.setPosition(static_cast<float>(static_cast<int>(posX - (this->sprite.getGlobalBounds().width) / 2)), this->sprite.getPosition().y);
}

void gui::Sprite::draw(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
