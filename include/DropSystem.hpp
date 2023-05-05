#ifndef DROPSYSTEM_HPP
#define DROPSYSTEM_HPP

#include "Drop.hpp"
#include "Player.hpp"
#include "PlayerGUI.hpp"
#include "FloatingTextSystem.hpp"
#include "SoundEngine.hpp"

class Drop;
class Player;
class PlayerGUI;
class FloatingTextSystem;
class SoundEngine;

class DropSystem
{
public:
	DropSystem(const float& difficulty_mod, const sf::VideoMode& vm);
	virtual ~DropSystem();

	void addDrop(const std::string& name, const float& posX, const float& posY, const uint32_t& worth);

	void update(Player* player, PlayerGUI* playerGui, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine, const float& dt);
	void draw(sf::RenderTarget& target);
private:
	std::list<std::unique_ptr<Drop>> drops;
	float difficulty_mod;
	sf::VideoMode vm;
};

#endif