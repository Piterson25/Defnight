#include "Shuriken.hpp"

Shuriken::Shuriken(const std::string &t_name, sf::VideoMode &t_vm,
                   const sf::Vector2f &t_position, float difficulty_mod,
                   const sf::Vector2f &coords, float coordsOffset,
                   Player &player)
    : Projectile(t_name, t_vm, t_position.x, t_position.y, difficulty_mod,
                 coords, coordsOffset)
{
    this->sprite.setTextureRect(sf::IntRect(4, 0, 4, 4));
    this->attack = player.getProjectileAttack();
    this->HP = 3;
    this->speed = 4;
    this->piercing = player.getProjectilePiercing();
}

Shuriken::~Shuriken() = default;

void Shuriken::playerCollision(Player &player)
{
}

void Shuriken::monsterCollision(Monster &monster, Player &player,
                                FloatingTextSystem &floatingTextSystem)
{
    const float distance = 2 * monster.getGlobalBounds().width;

    if (!monster.isPunched() &&
        vectorDistance(this->sprite.getPosition(), monster.getPosition()) <
            distance &&
        !this->collidedWall && !this->collidedPlayer) {

        sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
        sf::FloatRect mobBounds = monster.getGlobalBounds();

        sf::FloatRect nextPos = projectileBounds;
        nextPos.left += this->velocity.x;
        nextPos.top += this->velocity.y;

        if (mobBounds.intersects(nextPos)) {
            this->collidedMonster = true;
            if ((static_cast<uint32_t>(Random::Float() * 100.f) + 1) <=
                player.getCriticalChance()) {
                const int attack = 2 * this->attack;
                floatingTextSystem.addFloatingText(
                    gui::ORANGE, std::to_string(-attack), calcChar(16, vm),
                    monster.getPosition().x + calcX(32, vm),
                    monster.getPosition().y + calcY(32, vm), false);
                if (static_cast<int>(monster.getHP() - attack) < 0) {
                    monster.setHP(0);
                }
                else {
                    monster.setHP(monster.getHP() - attack);
                }

                player.setDamageDealt(player.getDamageDealt() + attack);
            }
            else {
                const int attack = this->attack;
                floatingTextSystem.addFloatingText(
                    gui::WHITE, std::to_string(-attack), calcChar(16, vm),
                    monster.getPosition().x + calcX(32, vm),
                    monster.getPosition().y + calcY(32, vm), false);
                if (static_cast<int>(monster.getHP() - attack) < 0) {
                    monster.setHP(0);
                }
                else {
                    monster.setHP(monster.getHP() - attack);
                }

                player.setDamageDealt(player.getDamageDealt() + attack);
            }

            monster.punch();

            piercing -= 1;

            if (this->piercing == 0) {
                this->HP = 0;
            }
        }
    }
}
