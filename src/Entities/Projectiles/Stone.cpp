#include "Stone.hpp"

Stone::Stone(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
             float t_y, float difficulty_mod, const sf::Vector2f &coords,
             float coordsOffset)
    : Projectile(t_name, t_vm, t_x, t_y, difficulty_mod, coords, coordsOffset)
{
    this->sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
    this->attack = static_cast<uint32_t>(4 * difficulty_mod);
    this->HP = 1;
    this->speed = 2;
}

Stone::~Stone() = default;

void Stone::playerCollision(Player &player)
{

    const float distance = 2 * player.getGlobalBounds().width;

    if (vectorDistance(this->sprite.getPosition(), player.getPosition()) <
            distance &&
        !this->collidedWall && !this->collidedPlayer &&
        !this->collidedMonster) {

        sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
        sf::FloatRect playerBounds = player.getGlobalBounds();

        sf::FloatRect nextPos = projectileBounds;
        nextPos.left += this->velocity.x;
        nextPos.top += this->velocity.y;

        if (playerBounds.intersects(nextPos)) {
            if (hasCollidedBottom(projectileBounds, playerBounds)) {
                this->velocity.y = 0.f;
                this->sprite.setPosition(projectileBounds.left,
                                         playerBounds.top -
                                             projectileBounds.height);
                this->collidedPlayer = true;
            }
            else if (hasCollidedTop(projectileBounds, playerBounds)) {
                this->velocity.y = 0.f;
                this->sprite.setPosition(projectileBounds.left,
                                         playerBounds.top +
                                             playerBounds.height);
                this->collidedPlayer = true;
            }

            if (hasCollidedRight(projectileBounds, playerBounds)) {
                this->velocity.x = 0.f;
                this->sprite.setPosition(playerBounds.left -
                                             projectileBounds.width,
                                         projectileBounds.top);
                this->collidedPlayer = true;
            }
            else if (hasCollidedLeft(projectileBounds, playerBounds)) {
                this->velocity.x = 0.f;
                this->sprite.setPosition(playerBounds.left + playerBounds.width,
                                         projectileBounds.top);
                this->collidedPlayer = true;
            }
        }
    }
}

void Stone::monsterCollision(Monster &monster, Player &player,
                             FloatingTextSystem &floatingTextSystem)
{
}
