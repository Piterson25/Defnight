#include "FloatingText.hpp"
#include "Functions.hpp"

FloatingText::FloatingText(sf::Font *font, const std::string &text, const unsigned &charSize,
                           const float &posX, const float &posY, const sf::Color &color,
                           const bool &isgui, sf::VideoMode &vm)
    : gui(isgui)
{
    this->font = font;

    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setCharacterSize(charSize);
    this->text.setFillColor(sf::Color(color.r, color.g, color.b, 0));
    this->text.setPosition(posX, posY);
    this->velocity = sf::Vector2f(0.f, calcY(-32.f, vm));
    this->moveCooldown = 0.f;
}

FloatingText::~FloatingText()
{
}

const bool FloatingText::isOver() const
{
    return this->moveCooldown >= 1.5f;
}

const bool FloatingText::isGui() const
{
    return this->gui;
}

void FloatingText::update(const float &dt)
{
    if (this->moveCooldown < 1.5f) {
        this->moveCooldown += dt;
        this->text.move(0, this->velocity.y * dt);

        if (this->moveCooldown < 0.5f) {
            this->text.setFillColor(sf::Color(
                this->text.getFillColor().r, this->text.getFillColor().g,
                this->text.getFillColor().b, static_cast<sf::Uint8>(this->moveCooldown * 510.f)));
        }
        else if (this->moveCooldown >= 1.f) {
            this->text.setFillColor(
                sf::Color(this->text.getFillColor().r, this->text.getFillColor().g,
                          this->text.getFillColor().b,
                          static_cast<sf::Uint8>(255.f - (this->moveCooldown * 510.f))));
        }
        else {
            this->text.setFillColor(sf::Color(this->text.getFillColor().r,
                                              this->text.getFillColor().g,
                                              this->text.getFillColor().b, 255));
        }
    }
}

void FloatingText::draw(sf::RenderTarget &target)
{
    target.draw(this->text);
}
