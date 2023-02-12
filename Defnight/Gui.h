#ifndef GUI_H
#define GUI_H

namespace gui {

	enum button_states { BUTTON_IDLE, BUTTON_HOVER, BUTTON_PRESSED };

	class ButtonText
	{
	public:
		ButtonText(sf::Font* font, const std::string& text, const unsigned& charSize,
			const float& posX, const float& posY, const sf::Color& idleColor, const sf::Color& hoverColor, const bool& center);
		~ButtonText();

		const bool isPressed() const;
		const std::string getText() const;
		const float getWidth() const;

		void setPosition(const sf::Vector2f& position);
		void setText(const std::string& text);
		void center(const float& posX);

		void update(const sf::Vector2i& mousePosWindow);
		void draw(sf::RenderTarget& target);
	private:
		uint32_t buttonState;
		sf::Font* font;
		sf::Text text;

		sf::Color idleColor;
		sf::Color hoverColor;
	};

	class Text
	{
	public:
		Text(sf::Font* font, const std::string& text, const unsigned& charSize,
			const float& posX, const float& posY, const sf::Color& color, const bool& center);
		~Text();

		const std::string getText() const;
		const float getWidth() const;

		void setPosition(const sf::Vector2f& position);
		void setFillColor(const sf::Color& color);
		void setText(const std::string& text);
		void center(const float& posX);

		void draw(sf::RenderTarget& target);
	private:
		sf::Font* font;
		sf::Text text;
	};



	class ButtonSprite
	{
	public:
		ButtonSprite(const std::string& texturePath, const float& posX, const float& posY, const float& scale, const bool& center);
		~ButtonSprite();

		const bool isPressed() const;

		void setTransparent();
		void center(const float& posX);

		void update(const sf::Vector2i& mousePosWindow);
		void draw(sf::RenderTarget& target);
	private:
		uint16_t buttonState;
		sf::Texture texture;
		sf::Sprite sprite;
	};

	class Sprite
	{
	public:
		Sprite(const std::string& texturePath, const float& posX, const float& posY, const float& scale, const bool& center);
		Sprite(const sf::Texture& texture, const float& posX, const float& posY, const float& scale, const bool& center);
		Sprite(sf::Sprite& sprite, const float& posX, const float& posY, const float& scale, const bool& center);
		~Sprite();

		virtual const sf::FloatRect getGlobalBounds() const;
		virtual const sf::IntRect getTextureRect() const;
		void setColor(const sf::Color& color);
		void setTextureRect(const sf::IntRect& intRect);
		void center(const float& posX);

		void draw(sf::RenderTarget& target);
	private:
		sf::Texture texture;
		sf::Sprite sprite;
	};
}

#endif