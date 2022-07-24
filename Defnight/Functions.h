#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <math.h>
#include <stack>
#include <string>
#include <vector>
#include "Random.h"

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

void createRectangle(sf::RectangleShape& nazwa, const float& szerokosc, const float& wysokosc, const float& pozycjaX, const float& pozycjaY,
	const sf::Uint8& R, const sf::Uint8& G, const sf::Uint8& B);

void createTransparentRect(sf::RectangleShape& nazwa, const float& szerokosc, const float& wysokosc, const float& pozycjaX, const float& pozycjaY, const float& grubosc);

void createText(sf::Text& nazwa, const sf::Font& czcionka, const float& rozmiarCzcionki, const std::string& napis, const float& pozycjaX, const float& pozycjaY,
	const sf::Uint8& R, const sf::Uint8& G, const sf::Uint8& B);

void createSprite(sf::Sprite& sprite, sf::Texture& tekstura, const std::string& sciezka_tekstury, const float& skala);
void createSprite(sf::Sprite& sprite, sf::Texture& tekstura, const float& skala);
void createSprite(sf::Sprite& sprite, sf::Texture& tekstura, const std::string& sciezka_tekstury, const float& skala, const float& pozycjaX, const float& pozycjaY);
void createSprite(sf::Sprite& sprite, sf::Texture& tekstura, const float& skala, const float& pozycjaX, const float& pozycjaY);

void cameraMove(sf::Sprite& sprite, const float& scale, sf::Sprite& background, sf::View& view);

const float getAngle(float x1, float y1, float x2, float y2);

const float vectorDistance(const float x1, const float y1, const float x2, const float y2);
const float vectorDistance(sf::Vector2f vec1, sf::Vector2f vec2);

void center(sf::Text& text, const float& x);
void center(sf::Sprite& sprite, const float& x);
