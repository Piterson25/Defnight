#include "DropSystem.hpp"

DropSystem::DropSystem(sf::VideoMode &vm, float difficulty_mod)
    : difficulty_mod(difficulty_mod), vm(vm)
{
}

DropSystem::~DropSystem()
{
    this->drops.clear();
}

void DropSystem::addDrop(const std::string &name, float posX, float posY,
                         uint32_t worth)
{
    this->drops.emplace_back(std::make_unique<Drop>(
        this->vm, name, posX, posY, worth, this->difficulty_mod == 1.25f));
}

void DropSystem::update(Player &player, PlayerGUI &playerGUI,
                        FloatingTextSystem &floatingTextSystem,
                        SoundEngine &soundEngine, float dt)
{
    for (const auto &drop : this->drops) {
        drop->update(dt);
    }

    for (auto drop = this->drops.begin(); drop != this->drops.end();) {
        if ((*drop)->hasSpawned() &&
            ((*drop)->isPickedByPlayer(player, playerGUI, floatingTextSystem,
                                       soundEngine, dt) ||
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
