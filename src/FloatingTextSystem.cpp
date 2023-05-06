#include "FloatingTextSystem.hpp"

FloatingTextSystem::FloatingTextSystem(sf::Font *font, const sf::VideoMode &vm) : font(font), vm(vm)
{
}

FloatingTextSystem::~FloatingTextSystem()
{
    this->floatingTexts.clear();
}

void FloatingTextSystem::addFloatingText(const std::string &text, const unsigned &charSize,
                                         const float &posX, const float &posY,
                                         const sf::Color &color, const bool &isgui)
{
    this->floatingTexts.emplace_back(
        new FloatingText(this->font, text, charSize, posX, posY, color, isgui, this->vm));
}

void FloatingTextSystem::draw(sf::RenderTarget &target)
{
    for (const auto &text : floatingTexts) {
        if (!text->isGui()) {
            text->draw(target);
        }
    }
}

void FloatingTextSystem::drawGui(sf::RenderTarget &target)
{
    for (const auto &text : floatingTexts) {
        if (text->isGui()) {
            text->draw(target);
        }
    }
}

void FloatingTextSystem::update(const float &dt)
{
    for (const auto &text : this->floatingTexts) {
        text->update(dt);
    }

    for (auto text = this->floatingTexts.begin(); text != this->floatingTexts.end();) {
        if ((*text)->isOver()) {
            text = this->floatingTexts.erase(text);
        }
        else {
            ++text;
        }
    }
}
