#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "MonsterSystem.hpp"
#include "FloatingTextSystem.hpp"
#include "SoundEngine.hpp"

class MonsterSystem;
class FloatingTextSystem;
class SoundEngine;

class Particle
{
public:
	Particle(const sf::VideoMode& vm, const std::string& name, const float& x, const float& y, const std::uint32_t& attack);
	virtual ~Particle();

	virtual const bool getExploded() const;
	virtual const uint32_t getAttack() const;
	virtual const sf::FloatRect getGlobalBounds() const;

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::VideoMode vm;
	std::string name;

	uint32_t attack;

	float animationCooldown;
	uint32_t frame;

	bool exploded;
};

#endif