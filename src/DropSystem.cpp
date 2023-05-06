#include "DropSystem.hpp"

DropSystem::DropSystem(const float &difficulty_mod, const sf::VideoMode &vm)
    : difficulty_mod(difficulty_mod), vm(vm)
{
}

DropSystem::~DropSystem()
{
    this->drops.clear();
}

void DropSystem::addDrop(const std::string &name, const float &posX, const float &posY,
                         const uint32_t &worth)
{
    this->drops.emplace_back(
        new Drop(name, posX, posY, worth, this->difficulty_mod == 1.25f, this->vm));
}

void DropSystem::update(Player *player, PlayerGUI *playerGui,
                        FloatingTextSystem *floatingTextSystem, SoundEngine *soundEngine,
                        const float &dt)
{
    for (const auto &drop : this->drops) {
        drop->update(dt);
    }

    for (auto drop = this->drops.begin(); drop != this->drops.end();) {
        if ((*drop)->getSpawned() &&
            ((*drop)->playerPick(player, playerGui, floatingTextSystem, soundEngine, dt) ||
             (*drop)->hasVanished())) {
            drop = this->drops.erase(drop);
        }
        else {
            ++drop;
        }
    }
}

void DropSystem::draw(sf::RenderTarget &target)
{
    for (const auto &drop : this->drops) {
        drop->draw(target);
    }
}
