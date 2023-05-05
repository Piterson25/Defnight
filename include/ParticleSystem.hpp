#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.hpp"
#include "MonsterSystem.hpp"
#include "FloatingTextSystem.hpp"
#include "SoundEngine.hpp"

class Particle;
class MonsterSystem;
class FloatingTextSystem;
class SoundEngine;

class ParticleSystem
{
public:
	ParticleSystem(const sf::VideoMode& vm);
	virtual ~ParticleSystem();

	void addParticle(const std::string& name, const float& x, const float& y, const std::uint32_t& attack);

	void update(MonsterSystem* monsterSystem, FloatingTextSystem* floatingTextSystem, const float& dt);
	void draw(sf::RenderTarget& target);
private:
	std::list<std::unique_ptr<Particle>> particles;
	sf::VideoMode vm;
};

#endif