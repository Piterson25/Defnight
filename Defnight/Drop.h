#pragma once
class Drop
{
public:
	sf::Texture texture;
	sf::Sprite sprite;

	float scale;
	sf::Vector2f predkosc;
	float spinCooldown;
	unsigned worth;
	float angle;
	std::string name;

	void init(const float& scale, const sf::Vector2f& position, const std::string& name, const unsigned& worth);
	void spin(const float& dt);
	void move(const float& dt);

	Drop();
	virtual ~Drop();


};
