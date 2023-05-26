#pragma once

#include "Effects/Particle.hpp"
#include "FloatingTextSystem.hpp"

class ParticleSystem {
public:
    ParticleSystem(sf::VideoMode &vm);
    ~ParticleSystem();

    const std::vector<sf::FloatRect> getParticlesGlobalBounds() const;

    void addParticle(const std::string &name, float x, float y,
                     std::uint32_t attack);

    void update(float dt);
    void draw(sf::RenderTarget &target);

private:
    std::list<std::unique_ptr<Particle>> particles;
    std::vector<sf::FloatRect> particlesGlobalBounds;
    sf::VideoMode &vm;
};
