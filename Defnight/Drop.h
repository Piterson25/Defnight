#ifndef DROP_H
#define DROP_H

#include "Player.h"
#include "PlayerGUI.h"
#include "FloatingText.h"

class Player;
class PlayerGUI;
class FloatingText;

class Drop
{
public:
	Drop(const std::string& name, const float& posX, const float& posY, const unsigned& worth, sf::VideoMode& vm, const float& soundVolume);
	virtual ~Drop();

	virtual const std::string getName() const;
	virtual const bool getSpawned() const;

	void spawn(const float& dt);
	void spin(const float& dt);
	void move(const float& posX, const float& posY, const float& dt);
	const bool playerPick(Player* player, sf::Font* font, PlayerGUI* playerGUI, std::list<FloatingText*>& floatingTexts, const float& dt);

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
private:
	sf::Texture texture;
	sf::Sprite sprite;

	sf::Vector2f velocity;
	std::string name;

	float spinCooldown;
	unsigned worth;
	float angle;
	sf::VideoMode vm;
	sf::SoundBuffer buffer;
	sf::Sound sound;

	bool spawned;
	float spawnCountdown;
};

#endif