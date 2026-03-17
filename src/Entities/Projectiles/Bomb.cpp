#include "Bomb.hpp"

Bomb::Bomb(const std::string &t_name, sf::VideoMode &t_vm,
           const sf::Vector2f &t_position, float difficulty_mod,
           const sf::Vector2f &coords, float coordsOffset, Player &player)
    : Projectile(t_name, t_vm, t_position.x, t_position.y, difficulty_mod,
                 coords, coordsOffset)
{
    this->sprite.setTextureRect(sf::IntRect({8, 0}, {4, 4}));
    this->attack = player.getProjectileAttack();
    this->HP = 1;
    this->speed = 3;
    this->piercing = 1;
}

Bomb::~Bomb() = default;

void Bomb::playerCollision(Player &player)
{
}

void Bomb::monsterCollision(Monster &monster, Player &player,
                            FloatingTextSystem &floatingTextSystem)
{
    const float distance = 2 * monster.getGlobalBounds().size.x;

    if (vectorDistance(this->sprite.getPosition(), monster.getPosition()) <
            distance &&
        !this->collidedWall && !this->collidedPlayer &&
        !this->collidedMonster) {

        sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
        sf::FloatRect mobBounds = monster.getGlobalBounds();

        sf::FloatRect nextPos = projectileBounds;
        nextPos.position.x += this->velocity.x;
        nextPos.position.y += this->velocity.y;

        if (mobBounds.findIntersection(nextPos)) {
            if (hasCollidedBottom(projectileBounds, mobBounds)) {
                this->velocity.y = 0.f;
                this->sprite.setPosition({projectileBounds.position.x,
                                         mobBounds.position.y -
                                             projectileBounds.size.y});
                this->collidedPlayer = true;
            }
            else if (hasCollidedTop(projectileBounds, mobBounds)) {
                this->velocity.y = 0.f;
                this->sprite.setPosition({projectileBounds.position.x,
                                         mobBounds.position.y +
                                             mobBounds.size.y});
                this->collidedPlayer = true;
            }

            if (hasCollidedRight(projectileBounds, mobBounds)) {
                this->velocity.x = 0.f;
                this->sprite.setPosition({mobBounds.position.x -
                                             mobBounds.size.x,
                                         mobBounds.position.y});
                this->collidedPlayer = true;
            }
            else if (hasCollidedLeft(projectileBounds, mobBounds)) {
                this->velocity.x = 0.f;
                this->sprite.setPosition({mobBounds.position.x + mobBounds.size.x,
                                         projectileBounds.position.y});
                this->collidedPlayer = true;
            }
        }

        if (this->collidedMonster) {
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

            this->HP = 0;
        }
    }
}
