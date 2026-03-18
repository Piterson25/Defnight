#include "GUI.hpp"

namespace gui {

    static sf::VideoMode vm;
    static sf::Texture selectsTexture;

    void initVM(const sf::VideoMode &t_vm)
    {
        vm = t_vm;
    }

    void initFont()
    {
        font = new sf::Font();
        if (!font->openFromFile("assets/font/PressStart2P-vaV7.ttf")) {
            throw std::runtime_error("ERROR - COULDN'T FIND FONT");
        }
    }

    void initTextures()
    {
        if (!selectsTexture.loadFromFile("assets/textures/selects.png")) {
            throw std::runtime_error("ERROR - COULDN'T FIND SELECTS TEXTURE");
        }
    }

    void clean()
    {
        delete font;
        font = nullptr;
    }

    Text::Text(const std::string &text, unsigned charSize, float x, float y, const sf::Color &color, bool center)
        : text(*font, text, charSize)
    {
        this->text.setFillColor(color);
        this->text.setPosition({x, y});
        this->centered = center;
        if (centered) {
            this->center(x);
        }
    }

    const sf::Vector2f Text::getPosition() const
    {
        return this->text.getPosition();
    }

    const std::string Text::getText() const
    {
        return this->text.getString();
    }

    const float Text::getWidth() const
    {
        return static_cast<float>(this->text.getGlobalBounds().size.x);
    }

    void Text::move(float x, float y)
    {
        this->text.move({x, y});
    }

    void Text::setAlphaColor(uint8_t alpha)
    {
        this->text.setFillColor(
            sf::Color(this->text.getFillColor().r, this->text.getFillColor().g, this->text.getFillColor().b, alpha));
    }

    void Text::setPositionX(float x)
    {
        this->text.setPosition({x, this->text.getPosition().y});
    }

    const sf::Color Text::getFillColor() const
    {
        return this->text.getFillColor();
    }

    void Text::setPosition(const sf::Vector2f &position)
    {
        this->text.setPosition(position);
    }

    void Text::setPosition(float x, float y)
    {
        this->text.setPosition(sf::Vector2f(x, y));
    }

    void Text::setFillColor(const sf::Color &color)
    {
        this->text.setFillColor(color);
    }

    void Text::setText(const std::string &text)
    {
        this->text.setString(text);
    }

    void Text::center(float x)
    {
        this->text.setPosition(
            {static_cast<float>(static_cast<int>(x - static_cast<float>(this->text.getGlobalBounds().size.x) * 0.5f)),
             this->text.getPosition().y});
    }

    void Text::draw(sf::RenderTarget &target)
    {
        target.draw(this->text);
    }

    ShadowText::ShadowText(const std::string &text, unsigned charSize, float x, float y, const sf::Color &color,
                           bool center)
        : Text(text, charSize, x, y, color, center), shadowText(*font, text, charSize)
    {
        this->shadowText.setFillColor(sf::Color(GREY.r, GREY.b, GREY.b, 192));
        this->shadowText.setPosition({this->text.getPosition().x + this->text.getCharacterSize() / 8.f,
                                      this->text.getPosition().y + this->text.getCharacterSize() / 8.f});
    }

    void ShadowText::move(float x, float y)
    {
        this->text.move({x, y});
        this->shadowText.move({x, y});
    }

    void ShadowText::setAlphaColor(uint8_t alpha)
    {
        this->text.setFillColor(
            sf::Color(this->text.getFillColor().r, this->text.getFillColor().g, this->text.getFillColor().b, alpha));
        this->shadowText.setFillColor(sf::Color(this->shadowText.getFillColor().r, this->shadowText.getFillColor().g,
                                                this->shadowText.getFillColor().b, alpha / 2));
    }

    void ShadowText::setPositionX(float x)
    {
        this->text.setPosition({x, this->text.getPosition().y});
        this->shadowText.setPosition({x, this->shadowText.getPosition().y});
    }

    void ShadowText::setPosition(const sf::Vector2f &position)
    {
        this->text.setPosition(sf::Vector2f(position));
        this->shadowText.setPosition(
            {static_cast<float>(static_cast<int>(this->text.getPosition().x + this->text.getCharacterSize() * 0.125f)),
             static_cast<float>(
                 static_cast<int>(this->text.getPosition().y + this->text.getCharacterSize() * 0.125f))});
    }

    void ShadowText::setText(const std::string &text)
    {
        this->shadowText.setString(text);
        this->text.setString(text);
    }

    void ShadowText::center(float x)
    {
        this->text.setPosition(
            {static_cast<float>(static_cast<int>(x - static_cast<float>(this->text.getGlobalBounds().size.x) * 0.5f)),
             this->text.getPosition().y});
        this->shadowText.setPosition(
            {static_cast<float>(static_cast<int>(this->text.getPosition().x + this->text.getCharacterSize() * 0.125f)),
             static_cast<float>(
                 static_cast<int>(this->text.getPosition().y + this->text.getCharacterSize() * 0.125f))});
    }

    void ShadowText::draw(sf::RenderTarget &target)
    {
        target.draw(this->shadowText);
        target.draw(this->text);
    }

    ButtonText::ButtonText(const std::string &text, unsigned charSize, float x, float y, const sf::Color &idleColor,
                           const sf::Color &hoverColor, bool center)
        : Text(text, charSize, x, y, idleColor, center), buttonState(ButtonState::BUTTON_IDLE), idleColor(idleColor),
          hoverColor(hoverColor)
    {
        this->setFillColor(idleColor);
    }

    bool ButtonText::isPressed(const sf::Vector2i &mousePosWindow)
    {
        this->buttonState = ButtonState::BUTTON_IDLE;

        if (this->text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
            this->buttonState = ButtonState::BUTTON_HOVER;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                this->buttonState = ButtonState::BUTTON_PRESSED;
            }
        }

        switch (this->buttonState) {
            case ButtonState::BUTTON_IDLE:
                this->text.setFillColor(this->idleColor);
                break;
            case ButtonState::BUTTON_HOVER:
                this->text.setFillColor(this->hoverColor);
                break;
            case ButtonState::BUTTON_PRESSED:
                this->text.setFillColor(this->idleColor);
                break;
        }

        if (this->buttonState == ButtonState::BUTTON_PRESSED && !GameInputHandler::isMouseClick()) {
            GameInputHandler::setMouseClick(true);
            return true;
        }

        return false;
    }

    Sprite::Sprite(const std::string &texturePath, float x, float y, float scale, bool center)
        : texture(texturePath), sprite(texture)
    {
        this->texture = sf::Texture(texturePath);
        this->sprite = sf::Sprite(this->texture);
        this->sprite.setPosition({x, y});
        this->sprite.setScale({scale, scale});
        this->centered = center;
        if (center) {
            this->center(x);
        }
    }

    Sprite::Sprite(const sf::Texture &texture, float x, float y, float scale, bool center)
        : texture(texture), sprite(texture)
    {
        this->sprite = sf::Sprite(this->texture);
        this->sprite.setPosition({x, y});
        this->sprite.setScale({scale, scale});
        this->centered = center;
        if (center) {
            this->center(x);
        }
    }

    Sprite::Sprite(const sf::Texture &texture, float x, float y, float scale, bool center, const sf::IntRect &intRect)
        : texture(texture), sprite(texture)
    {
        this->sprite = sf::Sprite(this->texture);
        this->sprite.setPosition({x, y});
        this->sprite.setScale({scale, scale});
        this->sprite.setTextureRect(intRect);
        this->centered = center;
        if (center) {
            this->center(x);
        }
    }

    Sprite::Sprite(sf::Sprite &sprite, float x, float y, float scale, bool center) : sprite(sprite)
    {
        this->sprite.setPosition({x, y});
        this->sprite.setScale({scale, scale});
        this->centered = center;
        if (center) {
            this->center(x);
        }
    }

    const sf::Vector2f Sprite::getPosition() const
    {
        return this->sprite.getPosition();
    }

    const sf::FloatRect Sprite::getGlobalBounds() const
    {
        return this->sprite.getGlobalBounds();
    }

    const sf::IntRect Sprite::getTextureRect() const
    {
        return this->sprite.getTextureRect();
    }

    void Sprite::setAlphaColor(uint8_t alpha)
    {
        this->sprite.setColor(
            sf::Color(this->sprite.getColor().r, this->sprite.getColor().g, this->sprite.getColor().b, alpha));
    }

    void Sprite::setColor(const sf::Color &color)
    {
        this->sprite.setColor(color);
    }

    void Sprite::setPosition(const sf::Vector2f &position)
    {
        this->sprite.setPosition(position);
    }

    void Sprite::setPosition(float x, float y)
    {
        this->sprite.setPosition(sf::Vector2f(x, y));
    }

    void Sprite::setTextureRect(const sf::IntRect &intRect)
    {
        this->sprite.setTextureRect(intRect);
    }

    void Sprite::flipHorizontal()
    {
        this->sprite.setScale({-this->sprite.getScale().x, this->sprite.getScale().y});
    }

    void Sprite::center(float x)
    {
        this->sprite.setPosition({static_cast<float>(static_cast<int>(x - (this->sprite.getGlobalBounds().size.x) / 2)),
                                  this->sprite.getPosition().y});
    }

    void Sprite::draw(sf::RenderTarget &target)
    {
        target.draw(this->sprite);
    }

    ButtonSprite::ButtonSprite(const sf::IntRect &intRect, float x, float y, float scale, const sf::Color &idleColor,
                               const sf::Color &hoverColor, bool center)
        : Sprite(selectsTexture, x, y, scale, center, intRect), buttonState(ButtonState::BUTTON_IDLE),
          idleColor(idleColor), hoverColor(hoverColor)
    {
        this->sprite.setColor(idleColor);
    }

    const sf::Color ButtonSprite::getColor() const
    {
        return this->idleColor;
    }

    void ButtonSprite::setIdleColor(const sf::Color color)
    {
        this->idleColor = color;
    }

    bool ButtonSprite::isPressed(const sf::Vector2i &mousePosWindow)
    {
        this->buttonState = ButtonState::BUTTON_IDLE;

        if (this->sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
            this->buttonState = ButtonState::BUTTON_HOVER;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                this->buttonState = ButtonState::BUTTON_PRESSED;
            }
        }

        switch (this->buttonState) {
            case ButtonState::BUTTON_IDLE:
                this->sprite.setColor(this->idleColor);
                break;
            case ButtonState::BUTTON_HOVER:
                this->sprite.setColor(this->hoverColor);
                break;
            case ButtonState::BUTTON_PRESSED:
                this->sprite.setColor(this->hoverColor);
                break;
        }

        if (this->buttonState == ButtonState::BUTTON_PRESSED && !GameInputHandler::isMouseClick()) {
            GameInputHandler::setMouseClick(true);
            return true;
        }

        return false;
    }

} // namespace gui
