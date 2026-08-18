#pragma once

#include "Game/GameInputHandler.hpp"

namespace gui {

    const sf::Color WHITE = sf::Color(255, 255, 255);
    const sf::Color BLACK = sf::Color(0, 0, 0);
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

    const sf::IntRect RECT_DIFFICULTY = sf::IntRect({0, 0}, {400, 400});
    const sf::IntRect RECT_DIFFICULTY_EXTREME = sf::IntRect({0, 400}, {880, 96});
    const sf::IntRect RECT_MAP = sf::IntRect({400, 0}, {304, 304});
    const sf::IntRect RECT_BUTTON = sf::IntRect({400, 304}, {88, 88});
    const sf::IntRect RECT_ARROW = sf::IntRect({488, 304}, {16, 10});
    const sf::IntRect RECT_BURGER = sf::IntRect({488, 314}, {10, 10});
    const sf::IntRect RECT_SMALL_ARROW = sf::IntRect({498, 312}, {8, 14});

    enum class ButtonState { BUTTON_IDLE, BUTTON_HOVER, BUTTON_PRESSED };

    static inline sf::Font *font = nullptr;

    void initVM(const sf::VideoMode &t_vm);
    void initFont();
    void initTextures();
    void clean();

    class Text {
    public:
        Text(const std::string &text, unsigned charSize, float x, float y, const sf::Color &color, bool center);
        virtual ~Text() = default;

        const sf::Vector2f getPosition() const;
        const std::string getText() const;
        const float getWidth() const;
        const sf::Color getFillColor() const;

        virtual void move(float x, float y);
        virtual void setAlphaColor(uint8_t alpha);
        virtual void setPositionX(float x);
        virtual void setPosition(const sf::Vector2f &position);
        virtual void setPosition(float x, float y);
        void setFillColor(const sf::Color &color);
        virtual void setText(const std::string &text);

        virtual void center(float x);
        virtual void draw(sf::RenderTarget &target);

    protected:
        sf::Text text;
        bool centered;
    };

    class ShadowText : public Text {
    public:
        ShadowText(const std::string &text, unsigned charSize, float x, float y, const sf::Color &color, bool center);
        virtual ~ShadowText() = default;

        void move(float x, float y) override;
        void setAlphaColor(uint8_t alpha) override;
        void setPositionX(float x) override;
        void setPosition(const sf::Vector2f &position) override;
        void setText(const std::string &text) override;
        void center(float x) override;
        void draw(sf::RenderTarget &target) override;

    private:
        sf::Text shadowText;
    };

    class ButtonText : public Text {
    public:
        ButtonText(const std::string &text, unsigned charSize, float x, float y, const sf::Color &idleColor,
                   const sf::Color &hoverColor, bool center);
        virtual ~ButtonText() = default;

        bool isPressed(const sf::Vector2i &mousePosWindow);

    private:
        ButtonState buttonState;
        sf::Color idleColor;
        sf::Color hoverColor;
    };

    class Sprite {
    public:
        Sprite(const std::string &texturePath, float x, float y, float scale, bool center);
        Sprite(const sf::Texture &texture, float x, float y, float scale, bool center);
        Sprite(const sf::Texture &texture, float x, float y, float scale, bool center, const sf::IntRect &intRect);
        Sprite(sf::Sprite &sprite, float x, float y, float scale, bool center);
        virtual ~Sprite() = default;

        const sf::Vector2f getPosition() const;
        const sf::FloatRect getGlobalBounds() const;
        const sf::IntRect getTextureRect() const;

        void setAlphaColor(uint8_t alpha);
        void setColor(const sf::Color &color);
        void setPosition(const sf::Vector2f &position);
        void setPosition(float x, float y);
        void setTextureRect(const sf::IntRect &intRect);

        void flipHorizontal();
        void center(float x);
        void draw(sf::RenderTarget &target);

    protected:
        sf::Texture texture;
        sf::Sprite sprite;
        bool centered;
    };

    class ButtonSprite : public Sprite {
    public:
        ButtonSprite(const sf::IntRect &intRect, float t_x, float t_y, float scale, const sf::Color &idleColor,
                     const sf::Color &hoverColor, bool center);
        virtual ~ButtonSprite() = default;

        bool isPressed(const sf::Vector2i &mousePosWindow);
        const sf::Color getColor() const;
        void setIdleColor(const sf::Color color);

    private:
        ButtonState buttonState;
        sf::Color idleColor;
        sf::Color hoverColor;
    };

} // namespace gui
