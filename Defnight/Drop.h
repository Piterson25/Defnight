#ifndef DROP_H
#define DROP_H

#include "Player.h"
#include "PlayerGUI.h"
#include "FloatingTextSystem.h"
#include "SoundEngine.h"

class Player;
class PlayerGUI;
class FloatingTextSystem;
class SoundEngine;

class Drop
{
public:
	Drop(const std::string& name, const float& posX, const float& posY, const uint32_t& worth, const bool& vanishing, sf::VideoMode& vm);
	virtual ~Drop();

	virtual const std::string getName() const;
	virtual const bool getSpawned() const;
	virtual const bool hasVanished() const;

	void spawn(const float& dt);
	void spin(const float& dt);
	void move(const float& posX, const float& posY, const float& dt);
	const bool playerPick(Player* player, PlayerGUI* playerGUI, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine, const float& dt);

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
private:
	sf::Texture texture;
	sf::Sprite sprite;

	sf::Vector2f velocity;
	std::string name;

	float spinCooldown;
	uint32_t worth;
	float angle;
	sf::VideoMode vm;

	float vanishingCountdown;
	bool vanishing;
	bool spawned;
	float spawnCountdown;
};

#endif