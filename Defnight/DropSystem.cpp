#include "stdafx.h"
#include "DropSystem.h"

DropSystem::DropSystem(const sf::VideoMode& vm)
	:vm(vm)
{

}

DropSystem::~DropSystem()
{
	this->drops.clear();
}

void DropSystem::addDrop(const std::string& name, const float& posX, const float& posY, const unsigned& worth)
{
	this->drops.emplace_back(new Drop(name, posX, posY, worth, this->vm));
}

void DropSystem::update(Player* player, PlayerGUI* playerGui, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine, const float& dt)
{
	for (const auto& drop : this->drops) {
		drop->update(dt);
	}

	for (auto drop = this->drops.begin(); drop != this->drops.end();) {
		if ((*drop)->getSpawned() && (*drop)->playerPick(player, playerGui, floatingTextSystem, soundEngine, dt)) {
			drop = this->drops.erase(drop);
		}
		else ++drop;
	}
}

void DropSystem::draw(sf::RenderTarget& target)
{
	for (const auto& drop : this->drops) {
		drop->draw(target);
	}
}
