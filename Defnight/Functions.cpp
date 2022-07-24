#include "Functions.h"

void createRectangle(sf::RectangleShape& nazwa, const float& szerokosc, const float& wysokosc, const float& pozycjaX, const float& pozycjaY, const sf::Uint8& R, const sf::Uint8& G, const sf::Uint8& B)
{
	nazwa.setSize(sf::Vector2f(szerokosc, wysokosc));
	nazwa.setPosition(pozycjaX, pozycjaY);
	nazwa.setFillColor(sf::Color(R, G, B));
}

void createTransparentRect(sf::RectangleShape& nazwa, const float& szerokosc, const float& wysokosc, const float& pozycjaX, const float& pozycjaY, const float& grubosc)
{
	nazwa.setSize(sf::Vector2f(szerokosc, wysokosc));
	nazwa.setPosition(pozycjaX, pozycjaY);
	nazwa.setFillColor(sf::Color::Transparent);
	nazwa.setOutlineColor(sf::Color::Transparent);
	nazwa.setOutlineThickness(grubosc);
}

void createText(sf::Text& nazwa, const sf::Font& czcionka, const float& rozmiarCzcionki, const std::string& napis, const float& pozycjaX, const float& pozycjaY, const sf::Uint8& R, const sf::Uint8& G, const sf::Uint8& B)
{
	nazwa.setFont(czcionka);
	nazwa.setString(napis);
	nazwa.setCharacterSize(unsigned(rozmiarCzcionki));
	nazwa.setFillColor(sf::Color(R, G, B));
	nazwa.setPosition(pozycjaX, pozycjaY);
}

void createSprite(sf::Sprite& sprite, sf::Texture& tekstura, const std::string& sciezka_tekstury, const float& skala)
{
	tekstura.loadFromFile(sciezka_tekstury);
	sprite.setTexture(tekstura);
	sprite.setScale(skala, skala);
}

void createSprite(sf::Sprite& sprite, sf::Texture& tekstura, const float& skala)
{
	sprite.setTexture(tekstura);
	sprite.setScale(skala, skala);
}

void createSprite(sf::Sprite& sprite, sf::Texture& tekstura, const std::string& sciezka_tekstury, const float& skala, const float& pozycjaX, const float& pozycjaY)
{
	tekstura.loadFromFile(sciezka_tekstury);
	sprite.setTexture(tekstura);
	sprite.setScale(skala, skala);
	sprite.setPosition(sf::Vector2f(pozycjaX, pozycjaY));
}

void createSprite(sf::Sprite& sprite, sf::Texture& tekstura, const float& skala, const float& pozycjaX, const float& pozycjaY)
{
	sprite.setTexture(tekstura);
	sprite.setScale(skala, skala);
	sprite.setPosition(sf::Vector2f(pozycjaX, pozycjaY));
}

void cameraMove(sf::Sprite& sprite, const float& scale, sf::Sprite& background, sf::View& view)
{
	if ((sprite.getPosition().x <= 608 * scale ||
		sprite.getPosition().x >= background.getGlobalBounds().width - 672 * scale) ||
		(sprite.getPosition().y <= 200 * scale ||
			sprite.getPosition().y >= background.getGlobalBounds().height - 392 * scale)) {

		if ((sprite.getPosition().x <= 608 * scale ||
			sprite.getPosition().x >= background.getGlobalBounds().width - 672 * scale) &&
			(sprite.getPosition().y <= 200 * scale ||
				sprite.getPosition().y >= background.getGlobalBounds().height - 392 * scale))
		{
			view.setCenter(sf::Vector2f(view.getCenter().x, view.getCenter().y));
		}
		else {
			if (sprite.getPosition().x <= 608 * scale ||
				sprite.getPosition().x >= background.getGlobalBounds().width - 672 * scale) {
				view.setCenter(sf::Vector2f(view.getCenter().x, sprite.getPosition().y + 32 * scale));
			}
			if (sprite.getPosition().y <= 200 * scale ||
				sprite.getPosition().y >= background.getGlobalBounds().height - 392 * scale) {
				view.setCenter(sf::Vector2f(sprite.getPosition().x + 32 * scale, view.getCenter().y));
			}
		}
	}
	else view.setCenter(sf::Vector2f(sprite.getPosition().x + 32 * scale, sprite.getPosition().y + 32 * scale));
}

const float getAngle(float x1, float y1, float x2, float y2)
{
	sf::Vector2f up(0, 1);
	sf::Vector2f p0(x1, y1);
	sf::Vector2f p1(x2, y2);
	sf::Vector2f q = p1 - p0;

	float q_length = sqrtf(q.x * q.x + q.y * q.y);

	q.x /= q_length;
	q.y /= q_length;

	float scalar_product = q.y;

	float angle = acos(scalar_product) * 180.0f / 3.1415f;

	if (x2 - x1 < 0.0f)
		return angle;
	else
		return -angle;
}

const float vectorDistance(const float x1, const float y1, const float x2, const float y2)
{
	const float dx = x2 - x1;
	const float dy = y2 - y1;

	return float(sqrt(pow(dx, 2) + pow(dy, 2)));
}

const float vectorDistance(sf::Vector2f vec1, sf::Vector2f vec2)
{
	const sf::Vector2f dVec = vec2 - vec1;

	return float(sqrt(pow(dVec.x, 2) + pow(dVec.y, 2)));
}

void center(sf::Text& text, const float& x)
{
	text.setPosition(float(int(x - (text.getLocalBounds().width) / 2)), text.getPosition().y);
}

void center(sf::Sprite& sprite, const float& x)
{
	sprite.setPosition(float(int(x - (sprite.getLocalBounds().width) / 2)), sprite.getPosition().y);
}
