#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <math.h>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include "Random.h"

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

const float calcX(const float& x, const sf::VideoMode& vm);
const float calcY(const float& y, const sf::VideoMode& vm);

const float calcScale(const float& x, const sf::VideoMode& vm);
const unsigned calcChar(const float& size, const sf::VideoMode& vm);

const float getAngle(float x1, float y1, float x2, float y2);

const float vectorDistance(const float x1, const float y1, const float x2, const float y2);
const float vectorDistance(sf::Vector2f vec1, sf::Vector2f vec2);

void center(sf::Text& text, const float& x);
void center(sf::Sprite& sprite, const float& x);

const bool sight(const sf::FloatRect& rect,
    const sf::Vector2f& a_p1, const sf::Vector2f& a_p2);

#endif