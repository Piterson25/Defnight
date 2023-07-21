#include "FloatingTextSystem.hpp"

FloatingTextSystem::FloatingTextSystem(sf::VideoMode &vm) : vm(vm)
{
}

FloatingTextSystem::~FloatingTextSystem()
{
    this->floatingTexts.clear();
}

void FloatingTextSystem::addFloatingText(const std::string &type,
                                         const std::string &text,
                                         unsigned charSize, float posX,
                                         float posY, bool isgui)
{
    sf::Color color = sf::Color(255, 255, 255);

    if (type == "white") {
        color = sf::Color(255, 255, 255);
    }
    else if (type == "gold") {
        color = sf::Color(255, 246, 76);
    }
    else if (type == "red") {
        color = sf::Color(182, 60, 53);
    }
    else if (type == "orange") {
        color = sf::Color(233, 134, 39);
    }
    else if (type == "flamingo") {
        color = sf::Color(228, 92, 95);
    }

    this->floatingTexts.emplace_back(std::make_unique<FloatingText>(
        vm, text, charSize, posX, posY, color, isgui));
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
