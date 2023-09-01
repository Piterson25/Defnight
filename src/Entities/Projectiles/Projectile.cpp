#include "Projectile.hpp"

Projectile::Projectile(const std::string &t_name, sf::VideoMode &t_vm,
                       float t_x, float t_y, float difficulty_mod,
                       const sf::Vector2f &coords, float coordsOffset)
    : Entity(t_name, t_vm, t_x, t_y), particleCooldown(0.f)
{

    this->texture.loadFromFile("assets/textures/projectiles.png");
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
    this->sprite.setPosition(t_x, t_y);
    this->sprite.setOrigin(2.f, 2.f);

    this->timeExisting = 0.f;
    this->angle = coordsOffset;
    this->collidedWall = false;
    this->collidedPlayer = false;
    this->collidedMonster = false;
    this->piercing = 1;
    this->bouncing = false;

    if (this->name == "STONE") {
        this->sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
    }
    else if (this->name == "SHURIKEN") {
        this->sprite.setTextureRect(sf::IntRect(4, 0, 4, 4));
        this->bouncing = true;
    }
    else if (this->name == "BOMB") {
        this->sprite.setTextureRect(sf::IntRect(8, 0, 4, 4));
    }
    else if (this->name == "GROUNDWAVE") {
        this->sprite.setTextureRect(sf::IntRect(12, 0, 8, 8));
    }

    this->calculateVelocity(coords);
}

Projectile::~Projectile() = default;

const bool Projectile::hasCollidedWall() const
{
    return this->collidedWall;
}

const bool Projectile::hasCollidedPlayer() const
{
    return this->collidedPlayer;
}

const bool Projectile::hasCollidedMonster() const
{
    return this->collidedMonster;
}

const float Projectile::getTimeExisting() const
{
    return this->timeExisting;
}

const uint32_t Projectile::getPiercing() const
{
    return this->piercing;
}

const bool Projectile::hasExploded() const
{
    if (this->name == "BOMB") {
        return this->timeExisting >= 3.f;
    }
    return false;
}

const bool Projectile::isBomb() const
{
    return this->name == "BOMB";
}

const bool Projectile::isParticleCooldown(float dt)
{
    this->particleCooldown += dt;

    if (this->particleCooldown > 0.01f) {
        this->particleCooldown = 0.f;
        return true;
    }
    return false;
}

void Projectile::setPiercing(uint32_t t_piercing)
{
    this->piercing = t_piercing;
}

void Projectile::calculateVelocity(const sf::Vector2f &coords)
{
    const float addAngle =
        getAngle(this->sprite.getPosition().x, this->sprite.getPosition().y,
                 coords.x, coords.y) +
        90.f;
    if (addAngle + this->angle >= 270.f) {
        this->angle = -90.f + this->angle - (270.f - addAngle);
    }
    else if (addAngle + this->angle <= -90.f) {
        this->angle = 270.f + this->angle + (90.f + addAngle);
    }
    else {
        this->angle += addAngle;
    }
    this->sprite.setRotation(this->angle);
}

void Projectile::wallCollision(const std::vector<sf::FloatRect> &obstacles)
{
    this->collidedWall = false;

    const float distance = 2 * obstacles[0].width;

    for (const auto &obstacleBounds : obstacles) {
        if (vectorDistance(this->sprite.getPosition(),
                           sf::Vector2f(obstacleBounds.left,
                                        obstacleBounds.top)) < distance &&
            !this->collidedWall && !this->collidedPlayer) {

            checkWallCollision(obstacleBounds);
        }
    }
}

void Projectile::checkWallCollision(const sf::FloatRect &obstacleBounds)
{
    sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
    sf::FloatRect wallBounds = obstacleBounds;

    sf::FloatRect nextPos = projectileBounds;
    nextPos.left += this->velocity.x;
    nextPos.top += this->velocity.y;

    if (!wallBounds.intersects(nextPos)) {
        return;
    }

    if (hasCollidedBottom(projectileBounds, wallBounds)) {
        if (this->bouncing) {
            this->angle *= -1.f;
        }
        else {
            this->sprite.setPosition(projectileBounds.left,
                                     wallBounds.top - projectileBounds.height);
        }
        this->velocity.y = 0.f;
        this->collidedWall = true;
    }
    else if (hasCollidedTop(projectileBounds, wallBounds)) {
        if (this->bouncing) {
            this->angle *= -1.f;
        }
        else {
            this->sprite.setPosition(projectileBounds.left,
                                     wallBounds.top + wallBounds.height);
        }
        this->velocity.y = 0.f;
        this->collidedWall = true;
    }

    if (hasCollidedRight(projectileBounds, wallBounds)) {
        if (this->bouncing) {
            this->angle = 180.f - this->angle;
        }
        else {
            this->sprite.setPosition(wallBounds.left - projectileBounds.width,
                                     projectileBounds.top);
        }
        this->velocity.x = 0.f;
        this->collidedWall = true;
    }
    else if (hasCollidedLeft(projectileBounds, wallBounds)) {
        if (this->bouncing) {
            this->angle = 180.f - this->angle;
        }
        else {
            this->sprite.setPosition(wallBounds.left + wallBounds.width,
                                     projectileBounds.top);
        }
        this->velocity.x = 0.f;
        this->collidedWall = true;
    }

    if (this->collidedWall) {
        this->HP--;
    }
}

void Projectile::update(float dt)
{
    const float vel = (this->speed * 0.2f + 0.8f) * 16.f *
                      this->sprite.getGlobalBounds().width * dt;

    this->velocity.x = vel * cos((3.1415f / 180.f) * this->angle);
    this->velocity.y = vel * sin((3.1415f / 180.f) * this->angle);
    this->timeExisting += dt;
    if (this->name == "SHURIKEN") {
        this->sprite.rotate(90.f / dt);
    }
}
