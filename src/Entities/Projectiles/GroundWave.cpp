#include "GroundWave.hpp"

GroundWave::GroundWave(const std::string &t_name, sf::VideoMode &t_vm,
                       float t_x, float t_y, float difficulty_mod,
                       const sf::Vector2f &coords, float coordsOffset)
    : Projectile(t_name, t_vm, t_x, t_y, difficulty_mod, coords, coordsOffset)
{
    this->sprite.setTextureRect(sf::IntRect(12, 0, 4, 4));
    this->attack = static_cast<uint32_t>(7 * difficulty_mod);
    this->HP = 1;
    this->speed = 2;
}

GroundWave::~GroundWave() = default;

void GroundWave::playerCollision(Player &player)
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

void GroundWave::monsterCollision(Monster &monster, Player &player,
                                  FloatingTextSystem &floatingTextSystem)
{
}

void GroundWave::update(float dt)
{
    const float vel = (this->speed * 0.2f + 0.8f) * 16.f *
                      this->sprite.getGlobalBounds().width * dt;

    this->velocity.x = vel * cos((3.1415f / 180.f) * this->angle);
    this->velocity.y = vel * sin((3.1415f / 180.f) * this->angle);
    this->timeExisting += dt;
}

void GroundWave::draw(sf::RenderTarget &target)
{
    target.draw(this->sprite);
}
