#pragma once

#include "Effects/FloatingText.hpp"

class FloatingTextSystem {
public:
    FloatingTextSystem(sf::VideoMode &vm);
    ~FloatingTextSystem();
    void addFloatingText(const std::string &text, unsigned charSize, float posX, float posY,
                         const sf::Color &color, bool isgui);

    void draw(sf::RenderTarget &target);
    void drawGui(sf::RenderTarget &target);
    void update(float dt);

private:
    sf::VideoMode &vm;
    std::list<std::unique_ptr<FloatingText>> floatingTexts;
};
