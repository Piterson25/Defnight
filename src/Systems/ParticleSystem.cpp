#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(sf::VideoMode &vm) : vm(vm)
{
}

ParticleSystem::~ParticleSystem()
{
    this->particles.clear();
}

const std::vector<sf::FloatRect>
ParticleSystem::getParticlesGlobalBounds() const
{
    return this->particlesGlobalBounds;
}

void ParticleSystem::clearParticlesGlobalBounds()
{
    this->particlesGlobalBounds.clear();
}

void ParticleSystem::addParticle(const std::string &name, float x, float y,
                                 std::uint32_t attack, std::uint32_t area)
{
    this->particles.emplace_back(
        std::make_unique<Particle>(this->vm, name, x, y, attack, area));
}

void ParticleSystem::addSmallParticle(const sf::Vector2f &position,
                                      const sf::Vector2f &size,
                                      const sf::Color &color)
{
    sf::RectangleShape particle;
    particle.setPosition(position);
    particle.setFillColor(color);
    particle.setSize(size);
    particle.rotate(Random::Float() * 360.f - 90.f);
    smallParticles.push_back(particle);
}

void ParticleSystem::update(float dt)
{
    for (auto particle = this->particles.begin();
         particle != this->particles.end();) {
        (*particle)->update(dt);
        if ((*particle)->hasExploded()) {
            this->particlesGlobalBounds.emplace_back(
                (*particle)->getGlobalBounds());
            particle = this->particles.erase(particle);
        }
        else {
            ++particle;
        }
    }

    for (auto particle = smallParticles.begin();
         particle != smallParticles.end();) {
        if (particle->getFillColor().a <= 0) {
            particle = smallParticles.erase(particle);
        }
        else {
            particle->setFillColor(sf::Color(
                particle->getFillColor().r, particle->getFillColor().g,
                particle->getFillColor().b, particle->getFillColor().a - dt));
            particle++;
        }
    }
}

void ParticleSystem::draw(sf::RenderTarget &target)
{
    for (const auto &particle : this->particles) {
        particle->draw(target);
    }
}

void ParticleSystem::drawSmallParticles(sf::RenderTarget &target)
{
    for (const auto &particle : this->smallParticles) {
        target.draw(particle);
    }
}
