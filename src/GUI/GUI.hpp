#pragma once

#include "Utils/Functions.hpp"

namespace gui {

    enum button_states { BUTTON_IDLE, BUTTON_HOVER, BUTTON_PRESSED };

    void initVM(sf::VideoMode &t_vm);
    void initFont();

    class ButtonText {
    public:
        ButtonText(const std::string &text, unsigned charSize, float posX,
                   float posY, const sf::Color &idleColor,
                   const sf::Color &hoverColor, bool center);
        ~ButtonText();

        const bool isPressed() const;
        const std::string getText() const;
        const float getWidth() const;

        void setPosition(const sf::Vector2f &position);
        void setText(const std::string &text);

        void center(float posX);
        void update(const sf::Vector2i &mousePosWindow);
        void draw(sf::RenderTarget &target);

    private:
        uint32_t buttonState;
        sf::Text text;

        sf::Color idleColor;
        sf::Color hoverColor;
    };

    class Text {
    public:
        Text(const std::string &text, unsigned charSize, float posX, float posY,
             const sf::Color &color, bool center);
        ~Text();

        const std::string getText() const;
        const float getWidth() const;
        const sf::Color getFillColor() const;

        void move(float x, float y);
        void setPosition(const sf::Vector2f &position);
        void setFillColor(const sf::Color &color);
        void setText(const std::string &text);

        void center(float posX);
        void draw(sf::RenderTarget &target);

    private:
        sf::Text text;
    };

    class ButtonSprite {
    public:
        ButtonSprite(const std::string &texturePath, float posX, float posY,
                     float scale, bool center);
        ButtonSprite(const sf::Texture &texture, float posX, float posY,
                     float scale, bool center);
        ~ButtonSprite();

        const bool isPressed() const;
        const sf::IntRect getTextureRect() const;

        void setTransparent();
        void setTextureRect(const sf::IntRect &intRect);
        void center(float posX);

        void update(const sf::Vector2i &mousePosWindow);
        void draw(sf::RenderTarget &target);

    private:
        uint16_t buttonState;
        sf::Texture texture;
        sf::Sprite sprite;
    };

    class Sprite {
    public:
        Sprite(const std::string &texturePath, float posX, float posY,
               float scale, bool center);
        Sprite(const sf::Texture &texture, float posX, float posY, float scale,
               bool center);
        Sprite(sf::Sprite &sprite, float posX, float posY, float scale,
               bool center);
        ~Sprite();

        const sf::FloatRect getGlobalBounds() const;
        const sf::IntRect getTextureRect() const;

        void setColor(const sf::Color &color);
        void setTextureRect(const sf::IntRect &intRect);

        void flipHorizontal();
        void center(float posX);
        void draw(sf::RenderTarget &target);

    private:
        sf::Texture texture;
        sf::Sprite sprite;
    };
} // namespace gui
