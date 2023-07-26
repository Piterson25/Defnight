#pragma once

#include "GUI/GUI.hpp"

class FloatingText {
public:
    FloatingText(sf::VideoMode &vm, const std::string &text, unsigned charSize,
                 float posX, float posY, const sf::Color &color, bool isgui);
    ~FloatingText();

    const bool isOver() const;
    const bool isGui() const;

    void update(float dt);
    void draw(sf::RenderTarget &target);

private:
    sf::Vector2f velocity;
    std::unique_ptr<gui::Text> text;
    bool gui;

    float moveCooldown;
};
