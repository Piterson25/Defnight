#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "FloatingTextSystem.hpp"
#include "MonsterSystem.hpp"
#include "Particle.hpp"
#include "SoundEngine.hpp"

class Particle;
class MonsterSystem;
class FloatingTextSystem;
class SoundEngine;

class ParticleSystem {
public:
    ParticleSystem(const sf::VideoMode &vm);
    virtual ~ParticleSystem();

    void addParticle(const std::string &name, const float &x, const float &y,
                     const std::uint32_t &attack);

    void update(MonsterSystem *monsterSystem, FloatingTextSystem *floatingTextSystem,
                const float &dt);
    void draw(sf::RenderTarget &target);

private:
    std::list<std::unique_ptr<Particle>> particles;
    sf::VideoMode vm;
};

#endif