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

void ParticleSystem::addParticle(const std::string &name, float x, float y,
                                 std::uint32_t attack)
{
    this->particles.emplace_back(new Particle(this->vm, name, x, y, attack));
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
}

void ParticleSystem::draw(sf::RenderTarget &target)
{
    for (const auto &particle : this->particles) {
        particle->draw(target);
    }
}
