#include "stdafx.h"
#include "Functions.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const sf::VideoMode& vm)
	:vm(vm)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::addParticle(const std::string& name, const float& x, const float& y, const std::uint32_t& attack)
{
	this->particles.emplace_back(new Particle(this->vm, name, x, y, attack));
}

void ParticleSystem::update(MonsterSystem* monsterSystem, FloatingTextSystem* floatingTextSystem, const float& dt)
{
	for (const auto& particle : this->particles) {
		particle->update(dt);
		if (particle->getExploded()) monsterSystem->explosionAttack(&(*particle), floatingTextSystem);
	}

	for (auto particle = this->particles.begin(); particle != this->particles.end();) {
		if ((*particle)->getExploded()) {
	
			particle = this->particles.erase(particle);
		}
		else ++particle;
	}
}

void ParticleSystem::draw(sf::RenderTarget& target)
{
	for (const auto& particle : this->particles) {
		particle->draw(target);
	}
}
