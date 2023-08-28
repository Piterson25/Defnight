#pragma once

#include "Game/GameInputHandler.hpp"

namespace gui {

    const sf::Color WHITE = sf::Color(255, 255, 255);
    const sf::Color GOLD = sf::Color(255, 246, 76);
    const sf::Color RED = sf::Color(182, 60, 53);
    const sf::Color ORANGE = sf::Color(233, 134, 39);
    const sf::Color FLAMINGO = sf::Color(228, 92, 95);
    const sf::Color GREY = sf::Color(67, 69, 73);
    const sf::Color LIGHT_GREY = sf::Color(192, 192, 192);
    const sf::Color DARK_RED = sf::Color(94, 7, 17);
    const sf::Color GREEN = sf::Color(23, 74, 27);
    const sf::Color DARK_GREEN = sf::Color(0, 50, 33);
    const sf::Color YELLOW = sf::Color(255, 177, 8);
    const sf::Color LIME = sf::Color(127, 189, 57);
    const sf::Color LIGHT_BLUE = sf::Color(36, 174, 214);
    const sf::Color PURPLE = sf::Color(88, 74, 127);
    const sf::Color BROWN = sf::Color(120, 76, 73);
    const sf::Color BLUE = sf::Color(0, 64, 81);
    const sf::Color PINK = sf::Color(113, 43, 59);

    const sf::IntRect RECT_DIFFICULTY = sf::IntRect(0, 0, 384, 504);
    const sf::IntRect RECT_MAP = sf::IntRect(384, 0, 304, 304);
    const sf::IntRect RECT_BUTTON = sf::IntRect(384, 304, 88, 88);
    const sf::IntRect RECT_ARROW = sf::IntRect(384, 392, 16, 10);
    const sf::IntRect RECT_BURGER = sf::IntRect(384, 402, 10, 10);
    const sf::IntRect RECT_SMALL_ARROW = sf::IntRect(394, 400, 8, 14);

    enum button_states { BUTTON_IDLE, BUTTON_HOVER, BUTTON_PRESSED };

    void initVM(sf::VideoMode &t_vm);
    void initFont();
    void initTextures();

    class ButtonText {
    public:
        ButtonText(const std::string &text, unsigned charSize, float posX,
                   float posY, const sf::Color &idleColor,
                   const sf::Color &hoverColor, bool center);
        ~ButtonText();

        bool isPressed(const sf::Vector2i &mousePosWindow);

        const sf::Vector2f &getPosition() const;

        const std::string getText() const;
        const float getWidth() const;

        void setPosition(const sf::Vector2f &position);
        void setText(const std::string &text);

        void center(float posX);
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

        const sf::Vector2f &getPosition() const;
        const std::string getText() const;
        const float getWidth() const;
        const sf::Color getFillColor() const;

        void move(float x, float y);
        void setAlphaColor(sf::Uint8 alpha);
        void setPositionX(float x);
        void setPositionY(float y);
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
        ButtonSprite(const sf::IntRect &intRect, float t_x, float t_y,
                     float scale, bool center);
        ~ButtonSprite();

        bool isPressed(const sf::Vector2i &mousePosWindow);

        void flipHorizontal();

        const sf::Color getColor() const;
        const sf::IntRect getTextureRect() const;

        void setColor(const sf::Color &t_color);
        void center(float posX);

        void draw(sf::RenderTarget &target);

    private:
        uint16_t buttonState;
        sf::Texture texture;
        sf::Sprite sprite;
        sf::Color color;
    };

    class Sprite {
    public:
        Sprite(const std::string &texturePath, float posX, float posY,
               float scale, bool center);
        Sprite(const sf::Texture &texture, float posX, float posY, float scale,
               bool center);
        Sprite(const sf::Texture &texture, float posX, float posY, float scale,
               bool center, const sf::IntRect &intRect);
        Sprite(sf::Sprite &sprite, float posX, float posY, float scale,
               bool center);
        ~Sprite();

        const sf::Vector2f getPosition() const;
        const sf::FloatRect getGlobalBounds() const;
        const sf::IntRect getTextureRect() const;

        void setAlphaColor(sf::Uint8 alpha);
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
