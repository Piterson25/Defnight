#include "FloatingTextSystem.hpp"

FloatingTextSystem::FloatingTextSystem(sf::VideoMode &vm) : vm(vm)
{
}

FloatingTextSystem::~FloatingTextSystem()
{
    this->floatingTexts.clear();
}

void FloatingTextSystem::addFloatingText(const std::string &text,
                                         unsigned charSize, float posX,
                                         float posY, const sf::Color &color,
                                         bool isgui)
{
    this->floatingTexts.emplace_back(
        new FloatingText(vm, text, charSize, posX, posY, color, isgui));
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

void FloatingTextSystem::update(float dt)
{
    for (const auto &text : this->floatingTexts) {
        text->update(dt);
    }

    for (auto text = this->floatingTexts.begin();
         text != this->floatingTexts.end();) {
        if ((*text)->isOver()) {
            text = this->floatingTexts.erase(text);
        }
        else {
            ++text;
        }
    }
}
