#pragma once

#include "Utils/header.h"

const float calcX(float x, const sf::VideoMode &vm);
const float calcY(float y, const sf::VideoMode &vm);

const float calcScale(float x, const sf::VideoMode &vm);
const unsigned calcChar(unsigned size, const sf::VideoMode &vm);

const float getAngle(float x1, float y1, float x2, float y2);

const float vectorDistance(float x1, float y1, float x2, float y2);
const float vectorDistance(const sf::Vector2f &vec1, const sf::Vector2f &vec2);

void center(sf::Text &text, float x);
void center(sf::Sprite &sprite, float x);

const bool sight(const sf::FloatRect &rect, const sf::Vector2f &a_p1,
                 const sf::Vector2f &a_p2);
