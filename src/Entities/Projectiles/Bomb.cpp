#include "Bomb.hpp"

Bomb::Bomb(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
           float difficulty_mod, const sf::Vector2f &coords, float coordsOffset)
    : Projectile(t_name, t_vm, t_x, t_y, difficulty_mod, coords, coordsOffset)
{
    this->sprite.setTextureRect(sf::IntRect(4, 0, 4, 4));
    this->attack = 3;
    this->HP = 3;
    this->speed = 4;
}

Bomb::~Bomb() = default;

void Bomb::playerCollision(Player &player)
{
}

void Bomb::monsterCollision(Monster &monster, Player &player,
                            FloatingTextSystem &floatingTextSystem)
{
    const float distance = 2 * monster.getGlobalBounds().width;

    if (vectorDistance(this->sprite.getPosition(), monster.getPosition()) <
            distance &&
        !this->collided && !this->collidedPlayer && !this->collidedMonster) {

        sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
        sf::FloatRect mobBounds = monster.getGlobalBounds();

        sf::FloatRect nextPos = projectileBounds;
        nextPos.left += this->velocity.x;
        nextPos.top += this->velocity.y;

        if (mobBounds.intersects(nextPos)) {
            if (bottomCollision(projectileBounds, mobBounds)) {
                this->velocity.y = 0.f;
                this->sprite.setPosition(projectileBounds.left,
                                         mobBounds.top -
                                             projectileBounds.height);
                this->collidedMonster = true;
            }
            else if (topCollision(projectileBounds, mobBounds)) {
                this->velocity.y = 0.f;
                this->sprite.setPosition(projectileBounds.left,
                                         mobBounds.top + mobBounds.height);
                this->collidedMonster = true;
            }

            if (rightCollision(projectileBounds, mobBounds)) {
                this->velocity.x = 0.f;
                this->sprite.setPosition(mobBounds.left -
                                             projectileBounds.width,
                                         projectileBounds.top);
                this->collidedMonster = true;
            }
            else if (leftCollision(projectileBounds, mobBounds)) {
                this->velocity.x = 0.f;
                this->sprite.setPosition(mobBounds.left + mobBounds.width,
                                         projectileBounds.top);
                this->collidedMonster = true;
            }
        }

        if (this->collidedMonster) {
            if ((static_cast<uint32_t>(Random::Float() * 100.f) + 1) <=
                player.getCriticalChance()) {
                const int attack = 2 * this->attack;
                floatingTextSystem.addFloatingText(
                    std::to_string(-attack), calcChar(16, vm),
                    monster.getPosition().x + calcX(32, vm),
                    monster.getPosition().y + calcY(32, vm),
                    sf::Color(233, 134, 39), false);
                if (static_cast<int>(monster.getHP() - attack) < 0) {
                    monster.setHP(0);
                }
                else {
                    monster.setHP(monster.getHP() - attack);
                }
            }
            else {
                const int attack = this->attack;
                floatingTextSystem.addFloatingText(
                    std::to_string(-attack), calcChar(16, vm),
                    monster.getPosition().x + calcX(32, vm),
                    monster.getPosition().y + calcY(32, vm),
                    sf::Color(255, 255, 255), false);
                if (static_cast<int>(monster.getHP() - attack) < 0) {
                    monster.setHP(0);
                }
                else {
                    monster.setHP(monster.getHP() - attack);
                }
            }

            monster.punch();

            this->HP = 0;
        }
    }
}
