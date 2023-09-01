#pragma once

#include "Effects/Particle.hpp"
#include "FloatingTextSystem.hpp"

class ParticleSystem {
public:
    ParticleSystem(sf::VideoMode &vm);
    ~ParticleSystem();

    const std::vector<sf::FloatRect> getParticlesGlobalBounds() const;

    void clearParticlesGlobalBounds();

    void addParticle(const std::string &name, float x, float y,
                     std::uint32_t attack, std::uint32_t area);

    void addSmallParticle(const sf::Vector2f &position,
                          const sf::Vector2f &size, const sf::Color &color);

    void update(float dt);
    void draw(sf::RenderTarget &target);
    void drawSmallParticles(sf::RenderTarget &target);

private:
    std::list<std::unique_ptr<Particle>> particles;
    std::list<sf::RectangleShape> smallParticles;
    std::vector<sf::FloatRect> particlesGlobalBounds;
    sf::VideoMode &vm;
};
