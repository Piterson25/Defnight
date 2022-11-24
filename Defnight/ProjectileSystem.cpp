#include "stdafx.h"
#include "ProjectileSystem.h"
#include "Functions.h"

ProjectileSystem::ProjectileSystem(const sf::VideoMode& vm)
	:vm(vm)
{
	this->textures.loadFromFile("external/assets/projectiles.png");
}

ProjectileSystem::~ProjectileSystem()
{
	this->projectiles.clear();
}

void ProjectileSystem::addProjectile(const std::string& projectile_name, const float& x, const float& y,
	const uint32_t& attack, const uint32_t& HP, const uint32_t& speed, const sf::Vector2f& coords)
{
	this->projectiles.emplace_back(new Projectile(this->vm, projectile_name, this->textures, x, y, attack, HP, speed, coords));
}

void ProjectileSystem::update(Player* player, PlayerGUI* playerGui, MonsterSystem* monsterSystem, sf::Sprite& background, TileMap* tileMap, FloatingTextSystem* floatingTextSystem, const float& dt)
{
	for (const auto& proj : this->projectiles) {
		proj->wallCollision(tileMap);
		proj->playerCollision(player);
		monsterSystem->projectileCollision(&(*proj), player, floatingTextSystem);
		proj->update(dt);
	}
	for (auto proj = this->projectiles.begin(); proj != this->projectiles.end();) {
		if ((*proj)->getHP() == 0) {
			proj = this->projectiles.erase(proj);
		}
		else if ((*proj)->getCollidedPlayer()) {
			int attack = static_cast<int>(round((*proj)->getAttack() - ((*proj)->getAttack() * player->getArmor() * 0.05f)));

			if (attack > 0) {
				if (static_cast<int>(player->getHP() - attack) < 0) player->setHP(0);
				else player->setHP(player->getHP() - attack);

				player->punch();

				floatingTextSystem->addFloatingText("-" + std::to_string(attack), calcChar(16, vm), player->getPosition().x + calcX(32, vm), player->getPosition().y + calcY(32, vm), sf::Color(228, 92, 95), false);
				playerGui->update_HP();
			}

			proj = this->projectiles.erase(proj);
		}
		else if (!background.getGlobalBounds().contains((*proj)->getPosition())) {
			proj = this->projectiles.erase(proj);
		}
		else ++proj;
	}
}

void ProjectileSystem::draw(sf::RenderTarget& target)
{
	for (const auto& proj : this->projectiles) {
		proj->draw(target);
	}
}
