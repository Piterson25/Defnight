#include "GUI.hpp"

namespace gui {

    static sf::Font font;
    static sf::VideoMode vm;
    static sf::Texture selectsTexture;

    void initVM(sf::VideoMode &t_vm)
    {
        vm = t_vm;
    }

    void initFont()
    {
        if (!font.loadFromFile("assets/font/PressStart2P-vaV7.ttf")) {
            throw std::runtime_error("ERROR - COULDN'T FIND FONT");
        }
    }

    void initTextures()
    {
        if (!selectsTexture.loadFromFile("assets/textures/selects.png")) {
            throw std::runtime_error("ERROR - COULDN'T FIND SELECTS TEXTURE");
        }
    }

    ButtonText::ButtonText(const std::string &text, unsigned charSize,
                           float posX, float posY, const sf::Color &idleColor,
                           const sf::Color &hoverColor, bool center)
        : buttonState(BUTTON_IDLE), idleColor(idleColor), hoverColor(hoverColor)
    {
        this->text.setFont(font);
        this->text.setString(text);
        this->text.setCharacterSize(charSize);
        this->text.setFillColor(idleColor);
        this->text.setPosition(posX, posY);
        if (center) {
            this->center(posX);
        }
    }

    ButtonText::~ButtonText() = default;

    const sf::Vector2f &ButtonText::getPosition() const
    {
        return this->text.getPosition();
    }

    const bool ButtonText::isPressed() const
    {
        return this->buttonState == BUTTON_PRESSED;
    }

    const std::string ButtonText::getText() const
    {
        return this->text.getString();
    }

    const float ButtonText::getWidth() const
    {
        return this->text.getGlobalBounds().width;
    }

    void ButtonText::setPosition(const sf::Vector2f &position)
    {
        this->text.setPosition(position);
    }

    void ButtonText::setText(const std::string &text)
    {
        this->text.setString(text);
    }

    void ButtonText::center(float posX)
    {
        this->text.setPosition(
            static_cast<float>(static_cast<int>(
                posX - (this->text.getGlobalBounds().width) / 2)),
            this->text.getPosition().y);
    }

    void ButtonText::update(const sf::Vector2i &mousePosWindow)
    {
        this->buttonState = BUTTON_IDLE;

        if (this->text.getGlobalBounds().contains(
                static_cast<sf::Vector2f>(mousePosWindow))) {
            this->buttonState = BUTTON_HOVER;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                this->buttonState = BUTTON_PRESSED;
            }
        }

        switch (this->buttonState) {
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

    void ButtonText::draw(sf::RenderTarget &target)
    {
        target.draw(this->text);
    }

    Text::Text(const std::string &text, unsigned charSize, float posX,
               float posY, const sf::Color &color, bool center)
    {
        this->text.setFont(font);
        this->text.setString(text);
        this->text.setCharacterSize(charSize);
        this->text.setFillColor(color);
        this->text.setPosition(posX, posY);
        if (center) {
            this->center(posX);
        }
    }

    Text::~Text() = default;

    const sf::Vector2f &Text::getPosition() const
    {
        return this->text.getPosition();
    }

    const std::string Text::getText() const
    {
        return this->text.getString();
    }

    const float Text::getWidth() const
    {
        return this->text.getGlobalBounds().width;
    }

    void Text::move(float x, float y)
    {
        this->text.move(x, y);
    }

    void Text::setPositionX(float x)
    {
        this->text.setPosition(x, this->text.getPosition().y);
    }

    void Text::setPositionY(float y)
    {
        this->text.setPosition(this->text.getPosition().x, y);
    }

    const sf::Color Text::getFillColor() const
    {
        return this->text.getFillColor();
    }

    void Text::setPosition(const sf::Vector2f &position)
    {
        this->text.setPosition(position);
    }

    void Text::setFillColor(const sf::Color &color)
    {
        this->text.setFillColor(color);
    }

    void Text::setText(const std::string &text)
    {
        this->text.setString(text);
    }

    void Text::center(float posX)
    {
        this->text.setPosition(
            static_cast<float>(static_cast<int>(
                posX - (this->text.getGlobalBounds().width) / 2)),
            this->text.getPosition().y);
    }

    void Text::draw(sf::RenderTarget &target)
    {
        target.draw(this->text);
    }

    ButtonSprite::ButtonSprite(const sf::IntRect &intRect, float t_x, float t_y,
                               float scale, bool center)
        : buttonState(BUTTON_IDLE)
    {
        this->color = gui::GREY;
        this->sprite.setTexture(gui::selectsTexture);
        this->sprite.setColor(this->color);
        this->sprite.setPosition(t_x, t_y);
        this->sprite.setScale(scale, scale);
        this->sprite.setTextureRect(intRect);
        if (center) {
            this->center(t_x);
        }
    }

    ButtonSprite::~ButtonSprite() = default;

    const sf::Color ButtonSprite::getColor() const
    {
        return this->color;
    }

    const bool ButtonSprite::isPressed() const
    {
        return this->buttonState == BUTTON_PRESSED;
    }

    const sf::IntRect ButtonSprite::getTextureRect() const
    {
        return this->sprite.getTextureRect();
    }

    void ButtonSprite::setColor(const sf::Color &t_color)
    {
        this->buttonState = BUTTON_IDLE;
        this->color = t_color;
        this->sprite.setColor(this->color);
    }

    void ButtonSprite::center(float posX)
    {
        this->sprite.setPosition(
            static_cast<float>(static_cast<int>(
                posX - (this->sprite.getGlobalBounds().width) / 2)),
            this->sprite.getPosition().y);
    }

    void ButtonSprite::update(const sf::Vector2i &mousePosWindow)
    {
        this->buttonState = BUTTON_IDLE;

        if (this->sprite.getGlobalBounds().contains(
                static_cast<sf::Vector2f>(mousePosWindow))) {
            this->buttonState = BUTTON_HOVER;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                this->buttonState = BUTTON_PRESSED;
            }
        }

        switch (this->buttonState) {
            case BUTTON_IDLE:
                this->sprite.setColor(this->color);
                break;
            case BUTTON_HOVER:
                this->sprite.setColor(gui::WHITE);
                break;
            case BUTTON_PRESSED:
                this->sprite.setColor(gui::WHITE);
                break;
        }
    }

    void ButtonSprite::draw(sf::RenderTarget &target)
    {
        target.draw(this->sprite);
    }

    Sprite::Sprite(const std::string &texturePath, float posX, float posY,
                   float scale, bool center)
    {
        this->texture.loadFromFile(texturePath);
        this->sprite.setTexture(this->texture);
        this->sprite.setPosition(posX, posY);
        this->sprite.setScale(scale, scale);
        if (center) {
            this->center(posX);
        }
    }

    Sprite::Sprite(const sf::Texture &texture, float posX, float posY,
                   float scale, bool center)
        : texture(texture)
    {
        this->sprite.setTexture(this->texture);
        this->sprite.setPosition(posX, posY);
        this->sprite.setScale(scale, scale);
        if (center) {
            this->center(posX);
        }
    }

    Sprite::Sprite(const sf::Texture &texture, float posX, float posY,
                   float scale, bool center, const sf::IntRect &intRect)
        : Sprite(texture, posX, posY, scale, center)
    {
        this->sprite.setTextureRect(intRect);
    }

    Sprite::Sprite(sf::Sprite &sprite, float posX, float posY, float scale,
                   bool center)
        : sprite(sprite)
    {
        this->sprite.setPosition(posX, posY);
        this->sprite.setScale(scale, scale);
        if (center) {
            this->center(posX);
        }
    }

    Sprite::~Sprite() = default;

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

    void Sprite::setColor(const sf::Color &color)
    {
        this->sprite.setColor(color);
    }

    void Sprite::setTextureRect(const sf::IntRect &intRect)
    {
        this->sprite.setTextureRect(intRect);
    }

    void Sprite::flipHorizontal()
    {
        this->sprite.setScale(-this->sprite.getScale().x,
                              this->sprite.getScale().y);
    }

    void Sprite::center(float posX)
    {
        this->sprite.setPosition(
            static_cast<float>(static_cast<int>(
                posX - (this->sprite.getGlobalBounds().width) / 2)),
            this->sprite.getPosition().y);
    }

    void Sprite::draw(sf::RenderTarget &target)
    {
        target.draw(this->sprite);
    }

} // namespace gui
